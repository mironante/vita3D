#include "RawModel.hpp"
#include "../RenderEngine/utils.h"

namespace Models
{
    RawModel::RawModel(Vertex* vertices, int vertexCount, SceUID verticesUid, 
                       uint16_t* indices, int indexCount, SceUID indicesUid)
    {
        _vertices = vertices;
        _vertexCount = vertexCount;
        _verticesUid = verticesUid;

        _indices = indices;
        _indexCount = indexCount;
        _indicesUid = indicesUid;
    }

    RawModel::~RawModel()
    {
        gpu_unmap_free(_indicesUid);
        gpu_unmap_free(_verticesUid);
    }

    int RawModel::getVertexCount()
    {
        return _vertexCount;
    }

    Vertex* RawModel::getVertices()
    {
        return _vertices;
    }

    int RawModel::getIndexCount()
    {
        return _indexCount;
    }

    uint16_t* RawModel::getIndices()
    {
        return _indices;
    }
}