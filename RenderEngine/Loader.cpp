#include "Loader.hpp"
#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace RenderEngine
{
    Loader::Loader()
    {

    }

    Loader::~Loader()
    {

    }

    RawModel* Loader::loadModel(Vertex* vertices, int vertexCount, uint16_t* indices, int indexCount)
    {
        SceUID verticesUid;
        SceUID indicesUid;

        Vertex* gpuVertices = (Vertex*)gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, 
            (SceGxmMemoryAttribFlags)(SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE), vertexCount * sizeof(Vertex), &verticesUid);
        memcpy(gpuVertices, vertices, vertexCount * sizeof(Vertex));

        uint16_t* gpuIndices = (uint16_t*)gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, 
            (SceGxmMemoryAttribFlags)(SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE), indexCount * sizeof(uint16_t), &indicesUid);
        memcpy(gpuIndices, indices, indexCount * sizeof(uint16_t));

        return new RawModel(gpuVertices, vertexCount, verticesUid, gpuIndices, indexCount, indicesUid);
    }


    SceGxmTexture Loader::loadTexture(std::string fileName)
    {
        int width, height, bits;
        unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &bits, 4);
        int size = width * height * bits;
	
        SceUID dataUid;
        void *texture_data = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
		SCE_GXM_MEMORY_ATTRIB_READ, size, &dataUid);

	    memcpy(texture_data, data, size);
        stbi_image_free(data);

        SceGxmTexture texture;
        sceGxmTextureInitLinear(&texture, texture_data, SCE_GXM_TEXTURE_FORMAT_A8B8G8R8, width, height, 0);
        sceGxmTextureSetMinFilter(&texture, SCE_GXM_TEXTURE_FILTER_LINEAR);
        sceGxmTextureSetMagFilter(&texture, SCE_GXM_TEXTURE_FILTER_LINEAR);

        return texture;
    }
}