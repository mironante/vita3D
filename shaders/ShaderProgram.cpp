#include "ShaderProgram.hpp"

namespace Shaders
{
    ShaderProgram::ShaderProgram(SceGxmShaderPatcher* shaderPatcher)
    {
        _shaderPatcher = shaderPatcher;
    }

    void ShaderProgram::loadProgramm(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram)
    {
        vertexShaderId = loadShader(vertexProgram);
        fragmentShaderId = loadShader(fragmentProgram);

        const SceGxmProgram* _vertexProgram = sceGxmShaderPatcherGetProgramFromId(vertexShaderId);
        const SceGxmProgram* _fragmentProgram = sceGxmShaderPatcherGetProgramFromId(fragmentShaderId);

        
        unsigned int attributeCount;
        SceGxmVertexStream vertexStream;
        SceGxmVertexAttribute* vertexAttribute = bindAttributes(_vertexProgram, _fragmentProgram, &attributeCount, &vertexStream);

        getAllUniformLocations(_vertexProgram, _fragmentProgram);

        sceGxmShaderPatcherCreateVertexProgram(_shaderPatcher, vertexShaderId, vertexAttribute, attributeCount, &vertexStream, 1, &patchedVertexProgram);

        sceGxmShaderPatcherCreateFragmentProgram(_shaderPatcher, fragmentShaderId, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, 0,
            _fragmentProgram, &patchedFragmentProgram); 
    }

    ShaderProgram::~ShaderProgram()
    {
        sceGxmShaderPatcherReleaseVertexProgram(_shaderPatcher, patchedVertexProgram);
        sceGxmShaderPatcherReleaseFragmentProgram(_shaderPatcher, patchedFragmentProgram);
        sceGxmShaderPatcherUnregisterProgram(_shaderPatcher, vertexShaderId);
	    sceGxmShaderPatcherUnregisterProgram(_shaderPatcher, fragmentShaderId);
    }

    void ShaderProgram::start(SceGxmContext* context)
    {
        sceGxmSetVertexProgram(context, patchedVertexProgram);
        sceGxmSetFragmentProgram(context, patchedFragmentProgram);
    }

    void ShaderProgram::stop()
    {

    }

    const SceGxmProgramParameter* ShaderProgram::getUniformLocation(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram, std::string uniformName, bool isVertexProgram)
    {
        if (isVertexProgram)
        {
            return sceGxmProgramFindParameterByName(vertexProgram, uniformName.c_str());
        }
        else
        {
            return sceGxmProgramFindParameterByName(fragmentProgram, uniformName.c_str());
        }
    }

    void ShaderProgram::loadFloat(SceGxmContext * context, const SceGxmProgramParameter* location, float value)
    {
        void* floatBuffer;
        sceGxmReserveVertexDefaultUniformBuffer(context, &floatBuffer);
        sceGxmSetUniformDataF(floatBuffer, location, 0, 1, &value);
    }

    void ShaderProgram::loadVector(SceGxmContext * context, const SceGxmProgramParameter* location, vector3f value)
    {
        void* floatBuffer;
        float vector[] = {value.x, value.y, value.z};
        sceGxmReserveVertexDefaultUniformBuffer(context, &floatBuffer);
        sceGxmSetUniformDataF(floatBuffer, location, 0, 3, (float *)vector);        
    }

    void ShaderProgram::loadBoolean(SceGxmContext * context, const SceGxmProgramParameter* location, bool value)
    {
        float val = 0.0f;
        if (value) val = 1.0f;
        void* floatBuffer;
        sceGxmReserveVertexDefaultUniformBuffer(context, &floatBuffer);
        sceGxmSetUniformDataF(floatBuffer, location, 0, 1, &val);          
    }

    void ShaderProgram::loadMatrix(SceGxmContext * context, const SceGxmProgramParameter* location, matrix4x4 value)
    {
        void* floatBuffer;
        sceGxmReserveVertexDefaultUniformBuffer(context, &floatBuffer);
        sceGxmSetUniformDataF(floatBuffer, location, 0, 16, (float *)value);        
    }

    SceGxmShaderPatcherId ShaderProgram::loadShader(const SceGxmProgram * program)
    {
        SceGxmShaderPatcherId shaderPatcherId;
        sceGxmShaderPatcherRegisterProgram(_shaderPatcher, program, &shaderPatcherId);
        return shaderPatcherId;
    }
}