#ifndef LOADER_HPP
#define LOADER_HPP

#include <string>
#include <psp2/gxm.h>

#include "../models/RawModel.hpp"

using namespace Models;

namespace RenderEngine
{
    class Loader
    {
    public:
        Loader();
        ~Loader();

        RawModel* loadModel(Vertex* vertices, int vertexCount, uint16_t* indices, int indexCount);

        SceGxmTexture loadTexture(std::string fileName);

    private:
    };
}

#endif // LOADER_HPP