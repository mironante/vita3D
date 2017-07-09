#ifndef STATICSHADER_HPP
#define STATICSHADER_HPP

#include "ShaderProgram.hpp"
#include "../entities/Camera.hpp"
#include "../toolbox/Maths.hpp"


using namespace Toolbox;
using namespace Entities;

namespace Shaders
{
    class StaticShader : public ShaderProgram
    {
    public:
        StaticShader(SceGxmShaderPatcher* shaderPatcher);
        ~StaticShader();
        void loadTransformationMatrix(SceGxmContext* context, matrix4x4 matrix);
        void loadProjectionMatrix(SceGxmContext* context, matrix4x4 projection);
        void loadViewMatrix(SceGxmContext* context, Camera* camera);
    private:
        SceGxmVertexAttribute* bindAttributes(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram, 
            unsigned int* attributeCount, SceGxmVertexStream* vertexStream);

        void getAllUniformLocations(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram);



    private:
        Maths _maths;
        matrix4x4 _viewMatrix;
        const SceGxmProgramParameter* location_transformationMatrix;
        const SceGxmProgramParameter* location_projectionMatrix;
        const SceGxmProgramParameter* location_viewMatrix;
    };
}

#endif // STATICSHADER_HPP