#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <psp2/gxm.h>
#include "../models/TexturedModel.hpp"
#include "../entities/Entity.hpp"
#include "../shaders/StaticShader.hpp"
#include "../toolbox/Maths.hpp"

using namespace Models;
using namespace Entities;
using namespace Shaders;
using namespace Toolbox;

namespace RenderEngine
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void render(SceGxmContext* context, Entity* entity, StaticShader* shader);

        void createProjectionMatrix();
    private:
        Maths _maths;

        float _FOV;
        float _NEAR_PLANE;
        float _FAR_PLANE;
        matrix4x4 _projectionMatrix;
    };
}

#endif // RENDERER_HPP