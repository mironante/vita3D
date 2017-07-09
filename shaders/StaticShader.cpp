#include "StaticShader.hpp"
#include "../models/RawModel.hpp"

#include <string.h>
#include <stdlib.h>

extern unsigned char _binary_shaders_vertexShader_gxp_start;
extern unsigned char _binary_shaders_fragmentShader_gxp_start;
static const SceGxmProgram *const simpleVertexShader = (SceGxmProgram *)&_binary_shaders_vertexShader_gxp_start;
static const SceGxmProgram *const simpleFragmentShader = (SceGxmProgram *)&_binary_shaders_fragmentShader_gxp_start;

namespace Shaders
{
    StaticShader::StaticShader(SceGxmShaderPatcher* shaderPatcher) : ShaderProgram(shaderPatcher)
    {
        loadProgramm(simpleVertexShader, simpleFragmentShader);
    }

    StaticShader::~StaticShader()
    {

    }

    SceGxmVertexAttribute* StaticShader::bindAttributes(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram, 
        unsigned int* attributeCount, SceGxmVertexStream* vertexStream)
    {
        const SceGxmProgramParameter* positionParam = sceGxmProgramFindParameterByName(vertexProgram, "position");
        const SceGxmProgramParameter* textureCoordsParam = sceGxmProgramFindParameterByName(vertexProgram, "textureCoords");

        int _attributeCount = 2;
        SceGxmVertexAttribute* vertexAttribute = new SceGxmVertexAttribute[_attributeCount];
        vertexAttribute[0].streamIndex = 0;
        vertexAttribute[0].offset = 0;
        vertexAttribute[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
        vertexAttribute[0].componentCount = 3;
        vertexAttribute[0].regIndex = sceGxmProgramParameterGetResourceIndex(positionParam);

        vertexAttribute[1].streamIndex = 0;
        vertexAttribute[1].offset = 3 * sizeof(float);
        vertexAttribute[1].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
        vertexAttribute[1].componentCount = 2;
        vertexAttribute[1].regIndex = sceGxmProgramParameterGetResourceIndex(textureCoordsParam);
        *attributeCount = _attributeCount;

        vertexStream->stride = sizeof(Models::Vertex);
        vertexStream->indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

        return vertexAttribute;
    }

    void StaticShader::getAllUniformLocations(const SceGxmProgram* vertexProgram, const SceGxmProgram* fragmentProgram)
    {
        location_transformationMatrix = getUniformLocation(vertexProgram, fragmentProgram, "transformationMatrix", true);
        location_projectionMatrix = getUniformLocation(vertexProgram, fragmentProgram, "projectionMatrix", true);
        location_viewMatrix = getUniformLocation(vertexProgram, fragmentProgram, "viewMatrix", true);
    }

    void StaticShader::loadTransformationMatrix(SceGxmContext* context, matrix4x4 matrix)
    {
        loadMatrix(context, location_transformationMatrix, matrix);
    }

    void StaticShader::loadProjectionMatrix(SceGxmContext* context, matrix4x4 projection)
    {
        loadMatrix(context, location_projectionMatrix, projection);
    }

    void StaticShader::loadViewMatrix(SceGxmContext* context, Camera* camera)
    {
        float *data = _maths.createViewMatrix(camera);

        memcpy(_viewMatrix, data, 16 * sizeof(float));
        delete [] data;
        
        loadMatrix(context, location_viewMatrix, _viewMatrix);
    }
}