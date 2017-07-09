#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include "Loader.hpp"
#include "../models/RawModel.hpp"
#include <string>


using namespace Models;

namespace RenderEngine
{
    class ObjLoader
    {
    public:
        static RawModel* loadObjModel(std::string fileName, Loader* loader);
    private:
    };
}

#endif // OBJ_LOADER_HPP