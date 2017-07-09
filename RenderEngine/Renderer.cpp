#include "Renderer.hpp"

#include <string.h>
#include <stdlib.h>

namespace RenderEngine
{
    Renderer::Renderer()
    {
        _FOV = 70.0f;
        _NEAR_PLANE = 0.1f;
        _FAR_PLANE = 1000.0f;
        createProjectionMatrix();
    }

    Renderer::~Renderer()
    {

    }

    void Renderer::render(SceGxmContext* context, Entity* entity, StaticShader* shader)
    {
        TexturedModel* model = entity->getModel();
        RawModel* rawModel = model->getRawModel();
        SceGxmTexture texture = model->getTexture()->getModelTexture();

        matrix4x4 transformationMatrix;

        float *data = _maths.createTransformationMatrix(entity->getPosition(), entity->getRotX(), entity->getRotY(), entity->getRotZ(), entity->getScale());
        memcpy(transformationMatrix, data, 16 * sizeof(float));
        delete [] data;
     
        shader->loadTransformationMatrix(context, transformationMatrix);
        shader->loadProjectionMatrix(context, _projectionMatrix);

        sceGxmSetFragmentTexture(context, 0, &texture);
        sceGxmSetVertexStream(context, 0, rawModel->getVertices());
	    sceGxmDraw(context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, rawModel->getIndices(), rawModel->getIndexCount());
    }

    void Renderer::createProjectionMatrix()
    {
        matrix4x4_init_perspective(_projectionMatrix, _FOV, 960.0f / 544.0f, _NEAR_PLANE, _FAR_PLANE);
    }
}