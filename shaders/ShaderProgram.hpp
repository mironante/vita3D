#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <psp2/gxm.h>
#include <string>
#include "../toolbox/math_utils.h"

namespace Shaders
{
    class ShaderProgram
    {
    public:
        ShaderProgram(SceGxmShaderPatcher* shaderPatcher);
        ~ShaderProgram();

        void loadProgramm(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram);

        void start(SceGxmContext* context);
        void stop();

    protected:
        virtual void getAllUniformLocations(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram) = 0;

        const SceGxmProgramParameter* getUniformLocation(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram, std::string uniformName, bool isVertexProgram);

        virtual SceGxmVertexAttribute* bindAttributes(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram, 
            unsigned int* attributeCount, SceGxmVertexStream* vertexStream) = 0;

        void loadFloat(SceGxmContext * context, const SceGxmProgramParameter* location, float value);
        void loadVector(SceGxmContext * context, const SceGxmProgramParameter* location, vector3f value, bool isVertexProgram);
        void loadBoolean(SceGxmContext * context, const SceGxmProgramParameter* location, bool value);
        void loadMatrix(SceGxmContext * context, const SceGxmProgramParameter* location, matrix4x4 value);

    private:
        SceGxmShaderPatcherId loadShader(const SceGxmProgram * program);

        SceGxmShaderPatcherId vertexShaderId;
        SceGxmShaderPatcherId fragmentShaderId;

        SceGxmVertexProgram* patchedVertexProgram;
        SceGxmFragmentProgram* patchedFragmentProgram;

        SceGxmShaderPatcher* _shaderPatcher;
    };
}

#endif // SHADERPROGRAM_HPP