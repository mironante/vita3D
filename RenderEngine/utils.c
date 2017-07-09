#include "utils.h"

void *gpu_alloc_map(SceKernelMemBlockType type, SceGxmMemoryAttribFlags gpu_attrib, size_t size, SceUID *uid)
{
    SceUID memuid;
    void *addr;

    if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW)
        size = ALIGN(size, 256 * 1024);
    else
        size = ALIGN(size, 4 * 1024);

    memuid = sceKernelAllocMemBlock("gpumem", type, size, NULL);
    if (memuid < 0)
        return NULL;

    if (sceKernelGetMemBlockBase(memuid, &addr) < 0)
        return NULL;

    if (sceGxmMapMemory(addr, size, gpu_attrib) < 0) {
        sceKernelFreeMemBlock(memuid);
        return NULL;
    }

    if (uid)
        *uid = memuid;

    return addr;
}

void gpu_unmap_free(SceUID uid)
{
    void *addr;

    if (sceKernelGetMemBlockBase(uid, &addr) < 0)
        return;

    sceGxmUnmapMemory(addr);

    sceKernelFreeMemBlock(uid);
}

void *gpu_vertex_usse_alloc_map(size_t size, SceUID *uid, unsigned int *usse_offset)
{
    SceUID memuid;
    void *addr;

    size = ALIGN(size, 4 * 1024);

    memuid = sceKernelAllocMemBlock("gpu_vertex_usse",
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, size, NULL);
    if (memuid < 0)
        return NULL;

    if (sceKernelGetMemBlockBase(memuid, &addr) < 0)
        return NULL;

    if (sceGxmMapVertexUsseMemory(addr, size, usse_offset) < 0)
        return NULL;

    return addr;
}

void gpu_vertex_usse_unmap_free(SceUID uid)
{
    void *addr;

    if (sceKernelGetMemBlockBase(uid, &addr) < 0)
        return;

    sceGxmUnmapVertexUsseMemory(addr);

    sceKernelFreeMemBlock(uid);
}

void *gpu_fragment_usse_alloc_map(size_t size, SceUID *uid, unsigned int *usse_offset)
{
    SceUID memuid;
    void *addr;

    size = ALIGN(size, 4 * 1024);

    memuid = sceKernelAllocMemBlock("gpu_fragment_usse",
        SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, size, NULL);
    if (memuid < 0)
        return NULL;

    if (sceKernelGetMemBlockBase(memuid, &addr) < 0)
        return NULL;

    if (sceGxmMapFragmentUsseMemory(addr, size, usse_offset) < 0)
        return NULL;

    return addr;
}

void gpu_fragment_usse_unmap_free(SceUID uid)
{
    void *addr;

    if (sceKernelGetMemBlockBase(uid, &addr) < 0)
        return;

    sceGxmUnmapFragmentUsseMemory(addr);

    sceKernelFreeMemBlock(uid);
}

void *shader_patcher_host_alloc_cb(void *user_data, unsigned int size)
{
    return malloc(size);
}

void shader_patcher_host_free_cb(void *user_data, void *mem)
{
    return free(mem);
}