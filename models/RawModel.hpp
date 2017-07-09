#ifndef RAWMODEL_HPP
#define RAWMODEL_HPP

#include <psp2/types.h>

namespace Models
{
    typedef struct Vertex
    {
        float x, y, z;
        float u, v;
        float nx, ny, nz;
    } Vertex;

    class RawModel
    {
    public:
        RawModel(Vertex* vertices, int vertexCount, SceUID verticesUid, 
                 uint16_t* indices, int indicesCount, SceUID indicesUid);
        ~RawModel();

        int getVertexCount();
        Vertex* getVertices();

        int getIndexCount();
        uint16_t* getIndices();

    private:
        int _vertexCount;
        Vertex* _vertices;
        SceUID _verticesUid;

        int _indexCount;
        uint16_t* _indices;
        SceUID _indicesUid;
    };
}

#endif // RAWMODEL_HPP