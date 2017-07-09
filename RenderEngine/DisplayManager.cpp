#include <psp2/kernel/sysmem.h>

#include <string.h>
#include <stdlib.h>

#include "DisplayManager.hpp"
#include "utils.h"
#include "../models/RawModel.hpp"

using namespace Models;

extern unsigned char _binary_shaders_clear_v_gxp_start;
extern unsigned char _binary_shaders_clear_f_gxp_start;
static const SceGxmProgram *const gxm_program_clear_v = (SceGxmProgram *)&_binary_shaders_clear_v_gxp_start;
static const SceGxmProgram *const gxm_program_clear_f = (SceGxmProgram *)&_binary_shaders_clear_f_gxp_start;


namespace RenderEngine
{
    // PSVITA special
    static SceUID vdm_ring_buffer_uid;
    static void *vdm_ring_buffer_addr;
    static SceUID vertex_ring_buffer_uid;
    static void *vertex_ring_buffer_addr;
    static SceUID fragment_ring_buffer_uid;
    static void *fragment_ring_buffer_addr;
    static SceUID fragment_usse_ring_buffer_uid;
    static void *fragment_usse_ring_buffer_addr;

    static SceGxmContext *gxm_context;

    static SceGxmRenderTarget *gxm_render_target;

    static SceGxmColorSurface gxm_color_surfaces[DISPLAY_BUFFER_COUNT];
    static SceUID gxm_color_surfaces_uid[DISPLAY_BUFFER_COUNT];
    static void *gxm_color_surfaces_addr[DISPLAY_BUFFER_COUNT];
    static SceGxmSyncObject *gxm_sync_objects[DISPLAY_BUFFER_COUNT];
    static unsigned int gxm_front_buffer_index;
    static unsigned int gxm_back_buffer_index;

    static SceUID gxm_depth_stencil_surface_uid;
    static void *gxm_depth_stencil_surface_addr;
    static SceGxmDepthStencilSurface gxm_depth_stencil_surface;

    static SceGxmShaderPatcher *gxm_shader_patcher;
    static SceUID gxm_shader_patcher_buffer_uid;
    static void *gxm_shader_patcher_buffer_addr;
    static SceUID gxm_shader_patcher_vertex_usse_uid;
    static void *gxm_shader_patcher_vertex_usse_addr;
    static SceUID gxm_shader_patcher_fragment_usse_uid;
    static void *gxm_shader_patcher_fragment_usse_addr;

    static SceGxmShaderPatcherId gxm_clear_vertex_program_id;
    static SceGxmShaderPatcherId gxm_clear_fragment_program_id;
    static const SceGxmProgramParameter *gxm_clear_vertex_program_position_param;
    static const SceGxmProgramParameter *gxm_clear_fragment_program_u_clear_color_param;
    static SceGxmVertexProgram *gxm_clear_vertex_program_patched;
    static SceGxmFragmentProgram *gxm_clear_fragment_program_patched;

    Vertex * clear_vertices_data;
    unsigned short * clear_indices_data;

    void displayQueueCallback(const void *callbackData);

    // DisplayManager
    DisplayManager::DisplayManager()
    {
        // Initialize
        SceGxmInitializeParams gxmInitParams;
        memset(&gxmInitParams, 0, sizeof(gxmInitParams));
        gxmInitParams.flags = 0;
        gxmInitParams.displayQueueMaxPendingCount = MAX_PENDING_SWAPS;
        gxmInitParams.displayQueueCallback = displayQueueCallback;
        gxmInitParams.displayQueueCallbackDataSize = sizeof(DisplayData);
        gxmInitParams.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;
        sceGxmInitialize(&gxmInitParams);

        vdm_ring_buffer_addr = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
            SCE_GXM_MEMORY_ATTRIB_READ, SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE,
            &vdm_ring_buffer_uid);

        vertex_ring_buffer_addr = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
            SCE_GXM_MEMORY_ATTRIB_READ, SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE,
            &vertex_ring_buffer_uid);

        fragment_ring_buffer_addr = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
            SCE_GXM_MEMORY_ATTRIB_READ, SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE,
            &fragment_ring_buffer_uid);

        unsigned int fragment_usse_offset;
        fragment_usse_ring_buffer_addr = gpu_fragment_usse_alloc_map(
            SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE,
            &fragment_ring_buffer_uid, &fragment_usse_offset);

        // Context
        SceGxmContextParams gxm_context_params;
        memset(&gxm_context_params, 0, sizeof(gxm_context_params));
        gxm_context_params.hostMem = malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
        gxm_context_params.hostMemSize = SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
        gxm_context_params.vdmRingBufferMem = vdm_ring_buffer_addr;
        gxm_context_params.vdmRingBufferMemSize = SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
        gxm_context_params.vertexRingBufferMem = vertex_ring_buffer_addr;
        gxm_context_params.vertexRingBufferMemSize = SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
        gxm_context_params.fragmentRingBufferMem = fragment_ring_buffer_addr;
        gxm_context_params.fragmentRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
        gxm_context_params.fragmentUsseRingBufferMem = fragment_usse_ring_buffer_addr;
        gxm_context_params.fragmentUsseRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
        gxm_context_params.fragmentUsseRingBufferOffset = fragment_usse_offset;
        sceGxmCreateContext(&gxm_context_params, &gxm_context);

        // Render target
        SceGxmRenderTargetParams render_target_params;
        memset(&render_target_params, 0, sizeof(render_target_params));
        render_target_params.flags = 0;
        render_target_params.width = DISPLAY_WIDTH;
        render_target_params.height = DISPLAY_HEIGHT;
        render_target_params.scenesPerFrame = 1;
        render_target_params.multisampleMode = SCE_GXM_MULTISAMPLE_NONE;
        render_target_params.multisampleLocations = 0;
        render_target_params.driverMemBlock = -1;
        sceGxmCreateRenderTarget(&render_target_params, &gxm_render_target);

        // Color surface
        for (int i = 0; i < DISPLAY_BUFFER_COUNT; i++) {
            gxm_color_surfaces_addr[i] = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
                (SceGxmMemoryAttribFlags)(SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE),
                ALIGN(4 * DISPLAY_STRIDE * DISPLAY_HEIGHT, 1 * 1024 * 1024),
                &gxm_color_surfaces_uid[i]);

            memset(gxm_color_surfaces_addr[i], 0, DISPLAY_STRIDE * DISPLAY_HEIGHT);

            sceGxmColorSurfaceInit(&gxm_color_surfaces[i],
                DISPLAY_COLOR_FORMAT,
                SCE_GXM_COLOR_SURFACE_LINEAR,
                SCE_GXM_COLOR_SURFACE_SCALE_NONE,
                SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,
                DISPLAY_WIDTH,
                DISPLAY_HEIGHT,
                DISPLAY_STRIDE,
                gxm_color_surfaces_addr[i]);

            sceGxmSyncObjectCreate(&gxm_sync_objects[i]);
        }

        // Depth surface
        unsigned int depth_stencil_width = ALIGN(DISPLAY_WIDTH, SCE_GXM_TILE_SIZEX);
        unsigned int depth_stencil_height = ALIGN(DISPLAY_HEIGHT, SCE_GXM_TILE_SIZEY);
        unsigned int depth_stencil_samples = depth_stencil_width * depth_stencil_height;

        gxm_depth_stencil_surface_addr = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
            (SceGxmMemoryAttribFlags)(SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE),
            4 * depth_stencil_samples, &gxm_depth_stencil_surface_uid);

        sceGxmDepthStencilSurfaceInit(&gxm_depth_stencil_surface, SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24, SCE_GXM_DEPTH_STENCIL_SURFACE_TILED,
            depth_stencil_width, gxm_depth_stencil_surface_addr, NULL);

        // Shader patcher
        static const unsigned int shader_patcher_buffer_size = 64 * 1024;
        static const unsigned int shader_patcher_vertex_usse_size = 64 * 1024;
        static const unsigned int shader_patcher_fragment_usse_size = 64 * 1024;

        gxm_shader_patcher_buffer_addr = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
            (SceGxmMemoryAttribFlags)(SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE),
            shader_patcher_buffer_size, &gxm_shader_patcher_buffer_uid);

        unsigned int shader_patcher_vertex_usse_offset;
        gxm_shader_patcher_vertex_usse_addr = gpu_vertex_usse_alloc_map(
            shader_patcher_vertex_usse_size, &gxm_shader_patcher_vertex_usse_uid,
            &shader_patcher_vertex_usse_offset);

        unsigned int shader_patcher_fragment_usse_offset;
        gxm_shader_patcher_fragment_usse_addr = gpu_fragment_usse_alloc_map(
            shader_patcher_fragment_usse_size, &gxm_shader_patcher_fragment_usse_uid,
            &shader_patcher_fragment_usse_offset);

        SceGxmShaderPatcherParams shader_patcher_params;
        memset(&shader_patcher_params, 0, sizeof(shader_patcher_params));
        shader_patcher_params.userData = NULL;
        shader_patcher_params.hostAllocCallback = shader_patcher_host_alloc_cb;
        shader_patcher_params.hostFreeCallback = shader_patcher_host_free_cb;
        shader_patcher_params.bufferAllocCallback = NULL;
        shader_patcher_params.bufferFreeCallback = NULL;
        shader_patcher_params.bufferMem = gxm_shader_patcher_buffer_addr;
        shader_patcher_params.bufferMemSize = shader_patcher_buffer_size;
        shader_patcher_params.vertexUsseAllocCallback = NULL;
        shader_patcher_params.vertexUsseFreeCallback = NULL;
        shader_patcher_params.vertexUsseMem = gxm_shader_patcher_vertex_usse_addr;
        shader_patcher_params.vertexUsseMemSize = shader_patcher_vertex_usse_size;
        shader_patcher_params.vertexUsseOffset = shader_patcher_vertex_usse_offset;
        shader_patcher_params.fragmentUsseAllocCallback = NULL;
        shader_patcher_params.fragmentUsseFreeCallback = NULL;
        shader_patcher_params.fragmentUsseMem = gxm_shader_patcher_fragment_usse_addr;
        shader_patcher_params.fragmentUsseMemSize = shader_patcher_fragment_usse_size;
        shader_patcher_params.fragmentUsseOffset = shader_patcher_fragment_usse_offset;
        sceGxmShaderPatcherCreate(&shader_patcher_params, &gxm_shader_patcher);

        // Clear programm
        sceGxmShaderPatcherRegisterProgram(gxm_shader_patcher, gxm_program_clear_v,
            &gxm_clear_vertex_program_id);
        sceGxmShaderPatcherRegisterProgram(gxm_shader_patcher, gxm_program_clear_f,
            &gxm_clear_fragment_program_id);

        const SceGxmProgram *clear_vertex_program =
            sceGxmShaderPatcherGetProgramFromId(gxm_clear_vertex_program_id);
        const SceGxmProgram *clear_fragment_program =
            sceGxmShaderPatcherGetProgramFromId(gxm_clear_fragment_program_id);

        gxm_clear_vertex_program_position_param = sceGxmProgramFindParameterByName(
            clear_vertex_program, "position");

        gxm_clear_fragment_program_u_clear_color_param = sceGxmProgramFindParameterByName(
            clear_fragment_program, "u_clear_color");

        SceGxmVertexAttribute clear_vertex_attribute;
        SceGxmVertexStream clear_vertex_stream;
        clear_vertex_attribute.streamIndex = 0;
        clear_vertex_attribute.offset = 0;
        clear_vertex_attribute.format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
        clear_vertex_attribute.componentCount = 2;
        clear_vertex_attribute.regIndex = sceGxmProgramParameterGetResourceIndex(
            gxm_clear_vertex_program_position_param);
        clear_vertex_stream.stride = sizeof(Vertex);
        clear_vertex_stream.indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

        sceGxmShaderPatcherCreateVertexProgram(gxm_shader_patcher,
            gxm_clear_vertex_program_id, &clear_vertex_attribute,
            1, &clear_vertex_stream, 1, &gxm_clear_vertex_program_patched);

        sceGxmShaderPatcherCreateFragmentProgram(gxm_shader_patcher,
            gxm_clear_fragment_program_id, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
            SCE_GXM_MULTISAMPLE_NONE, NULL, clear_fragment_program,
            &gxm_clear_fragment_program_patched);

        SceUID clear_vertices_uid;
        clear_vertices_data = (Vertex*)gpu_alloc_map(
            SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ,
            4 * sizeof(Vertex), &clear_vertices_uid);

        SceUID clear_indices_uid;
        clear_indices_data = (unsigned short*)gpu_alloc_map(
            SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ,
            4 * sizeof(unsigned short), &clear_indices_uid);

        clear_vertices_data[0] = (Vertex){-1.0f, -1.0f};
        clear_vertices_data[1] = (Vertex){ 1.0f, -1.0f};
        clear_vertices_data[2] = (Vertex){-1.0f,  1.0f};
        clear_vertices_data[3] = (Vertex){ 1.0f,  1.0f};

        clear_indices_data[0] = 0;
        clear_indices_data[1] = 1;
        clear_indices_data[2] = 2;
        clear_indices_data[3] = 3;
    }

    DisplayManager::~DisplayManager()
    {
        sceGxmDisplayQueueFinish();
        sceGxmFinish(gxm_context);

        sceGxmShaderPatcherDestroy(gxm_shader_patcher);

        gpu_unmap_free(gxm_shader_patcher_buffer_uid);
        gpu_vertex_usse_unmap_free(gxm_shader_patcher_vertex_usse_uid);
        gpu_fragment_usse_unmap_free(gxm_shader_patcher_fragment_usse_uid);

        gpu_unmap_free(gxm_depth_stencil_surface_uid);

        for (int i = 0; i < DISPLAY_BUFFER_COUNT; i++) {
            gpu_unmap_free(gxm_color_surfaces_uid[i]);
            sceGxmSyncObjectDestroy(gxm_sync_objects[i]);
        }

        sceGxmDestroyRenderTarget(gxm_render_target);

        gpu_unmap_free(vdm_ring_buffer_uid);
        gpu_unmap_free(vertex_ring_buffer_uid);
        gpu_unmap_free(fragment_ring_buffer_uid);
        gpu_fragment_usse_unmap_free(fragment_usse_ring_buffer_uid);

        sceGxmDestroyContext(gxm_context);

        sceGxmTerminate();
    }

    void DisplayManager::startFrame()
    {
		sceGxmBeginScene(gxm_context, 0, gxm_render_target, NULL, NULL, gxm_sync_objects[gxm_back_buffer_index],
			&gxm_color_surfaces[gxm_back_buffer_index], &gxm_depth_stencil_surface);
    }

    void DisplayManager::closeFrame()
    {
        sceGxmEndScene(gxm_context, NULL, NULL);
    }

    void DisplayManager::swapBuffers()
    {
		sceGxmPadHeartbeat(&gxm_color_surfaces[gxm_back_buffer_index],
			gxm_sync_objects[gxm_back_buffer_index]);

		DisplayData displayData;
		displayData.address = gxm_color_surfaces_addr[gxm_back_buffer_index];

		sceGxmDisplayQueueAddEntry(gxm_sync_objects[gxm_front_buffer_index],
			gxm_sync_objects[gxm_back_buffer_index], &displayData);

		gxm_front_buffer_index = gxm_back_buffer_index;
		gxm_back_buffer_index = (gxm_back_buffer_index + 1) % DISPLAY_BUFFER_COUNT;
    }

    SceGxmContext* DisplayManager::getContext()
    {
        return gxm_context;
    }

    SceGxmShaderPatcher* DisplayManager::getShaderPatcher()
    {
        return gxm_shader_patcher;
    }

    void DisplayManager::clearScreen(float r, float g, float b, float a)
    {
        sceGxmSetVertexProgram(gxm_context, gxm_clear_vertex_program_patched);
        sceGxmSetFragmentProgram(gxm_context, gxm_clear_fragment_program_patched);

        static const float clear_color[4] = {
            r, g, b, a
        };

        void *uniform_buffer;
        sceGxmReserveFragmentDefaultUniformBuffer(gxm_context, &uniform_buffer);
        sceGxmSetUniformDataF(uniform_buffer, gxm_clear_fragment_program_u_clear_color_param,
            0, sizeof(clear_color) / sizeof(float), clear_color);

        sceGxmSetVertexStream(gxm_context, 0, clear_vertices_data);
        sceGxmDraw(gxm_context, SCE_GXM_PRIMITIVE_TRIANGLE_STRIP,
            SCE_GXM_INDEX_FORMAT_U16, clear_indices_data, 4);

        static const float color[4] = {
            1.0f, 1.0f, 0.0f, 1.0f
        };

        sceGxmReserveFragmentDefaultUniformBuffer(gxm_context, &uniform_buffer);
        sceGxmSetUniformDataF(uniform_buffer, gxm_clear_fragment_program_u_clear_color_param,
            0, sizeof(color) / sizeof(float), color);
    }

    void displayQueueCallback(const void *callbackData)
    {
        SceDisplayFrameBuf frameBuffer;
        const DisplayData *displayData = (const DisplayData *)callbackData;

        memset(&frameBuffer, 0, sizeof(frameBuffer));
        frameBuffer.size        = sizeof(frameBuffer);
        frameBuffer.base        = displayData->address;
        frameBuffer.pitch       = DISPLAY_STRIDE;
        frameBuffer.pixelformat = DISPLAY_PIXEL_FORMAT;
        frameBuffer.width       = DISPLAY_WIDTH;
        frameBuffer.height      = DISPLAY_HEIGHT;

        sceDisplaySetFrameBuf(&frameBuffer, SCE_DISPLAY_SETBUF_NEXTFRAME);

        sceDisplayWaitVblankStart();
    }
}