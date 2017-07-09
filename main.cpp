#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>

#include "RenderEngine/DisplayManager.hpp"
#include "models/RawModel.hpp"
#include "models/TexturedModel.hpp"
#include "RenderEngine/Loader.hpp"
#include "RenderEngine/Renderer.hpp"
#include "shaders/StaticShader.hpp"
#include "textures/ModelTexture.hpp"
#include "toolbox/Maths.hpp"
#include "entities/Entity.hpp"
#include "entities/Camera.hpp"
#include "RenderEngine/ObjLoader.hpp"


using namespace RenderEngine;
using namespace Shaders;
using namespace Models;
using namespace Textures;
using namespace Toolbox;
using namespace Entities;

int main()
{
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData ctrl_peek;
	SceCtrlData ctrl_press;

    DisplayManager* DM = new DisplayManager();
    Loader* loader = new Loader();
    Renderer* renderer = new Renderer();   
    StaticShader* staticShader = new StaticShader(DM->getShaderPatcher());

    Camera* camera = new Camera();

    Vertex* vertices = new Vertex[24];
    vertices[0].x = -0.5f; vertices[0].y = 0.5f; vertices[0].z =  -0.5f;
    vertices[0].u =  0.0f; vertices[0].v =  0.0f;

    vertices[1].x = -0.5f; vertices[1].y = -0.5f; vertices[1].z =  -0.5f;
    vertices[1].u =  0.0f; vertices[1].v =  1.0f;

    vertices[2].x = 0.5f; vertices[2].y = -0.5f; vertices[2].z =  -0.5f;
    vertices[2].u =  1.0f; vertices[2].v =  1.0f;

    vertices[3].x = 0.5f; vertices[3].y = 0.5f; vertices[3].z =  -0.5f;
    vertices[3].u =  1.0f; vertices[3].v =  0.0f;

    vertices[4].x = -0.5f; vertices[4].y = 0.5f; vertices[4].z =  0.5f;
    vertices[4].u =  0.0f; vertices[4].v =  0.0f;

    vertices[5].x = -0.5f; vertices[5].y = -0.5f; vertices[5].z =  0.5f;
    vertices[5].u =  0.0f; vertices[5].v =  1.0f;

    vertices[6].x = 0.5f; vertices[6].y = -0.5f; vertices[6].z =  0.5f;
    vertices[6].u =  1.0f; vertices[6].v =  1.0f;

    vertices[7].x = 0.5f; vertices[7].y = 0.5f; vertices[7].z =  0.5f;
    vertices[7].u =  1.0f; vertices[7].v =  0.0f;

    vertices[8].x = 0.5f; vertices[8].y = 0.5f; vertices[8].z =  -0.5f;
    vertices[8].u =  0.0f; vertices[8].v =  0.0f;

    vertices[9].x = 0.5f; vertices[9].y = -0.5f; vertices[9].z =  -0.5f;
    vertices[9].u =  0.0f; vertices[9].v =  1.0f;

    vertices[10].x = 0.5f; vertices[10].y = -0.5f; vertices[10].z =  0.5f;
    vertices[10].u =  1.0f; vertices[10].v =  1.0f;

    vertices[11].x = 0.5f; vertices[11].y = 0.5f; vertices[11].z =  0.5f;
    vertices[11].u =  1.0f; vertices[11].v =  0.0f;

    vertices[12].x = -0.5f; vertices[12].y = 0.5f; vertices[12].z =  -0.5f;
    vertices[12].u =  0.0f; vertices[12].v =  0.0f;

    vertices[13].x = -0.5f; vertices[13].y = -0.5f; vertices[13].z =  -0.5f;
    vertices[13].u =  0.0f; vertices[13].v =  1.0f;

    vertices[14].x = -0.5f; vertices[14].y = -0.5f; vertices[14].z =  0.5f;
    vertices[14].u =  1.0f; vertices[14].v =  1.0f;

    vertices[15].x = -0.5f; vertices[15].y = 0.5f; vertices[15].z =  0.5f;
    vertices[15].u =  1.0f; vertices[15].v =  0.0f;

    vertices[16].x = -0.5f; vertices[16].y = 0.5f; vertices[16].z =  0.5f;
    vertices[16].u =  0.0f; vertices[16].v =  0.0f;

    vertices[17].x = -0.5f; vertices[17].y = 0.5f; vertices[17].z =  -0.5f;
    vertices[17].u =  0.0f; vertices[17].v =  1.0f;

    vertices[18].x = 0.5f; vertices[18].y = 0.5f; vertices[18].z =  -0.5f;
    vertices[18].u =  1.0f; vertices[18].v =  1.0f;

    vertices[19].x = 0.5f; vertices[19].y = 0.5f; vertices[19].z =  0.5f;
    vertices[19].u =  1.0f; vertices[19].v =  0.0f;

    vertices[20].x = -0.5f; vertices[20].y = -0.5f; vertices[20].z =  0.5f;
    vertices[20].u =  0.0f; vertices[20].v =  0.0f;

    vertices[21].x = -0.5f; vertices[21].y = -0.5f; vertices[21].z =  -0.5f;
    vertices[21].u =  0.0f; vertices[21].v =  1.0f;

    vertices[22].x = 0.5f; vertices[22].y = -0.5f; vertices[22].z =  -0.5f;
    vertices[22].u =  1.0f; vertices[22].v =  1.0f;

    vertices[23].x = 0.5f; vertices[23].y = -0.5f; vertices[23].z =  0.5f;
    vertices[23].u =  1.0f; vertices[23].v =  0.0f;

    uint16_t* indices = new uint16_t[36];
    indices[0] = 0; indices[1] = 1; indices[2] = 3;	
    indices[3] = 3; indices[4] = 1; indices[5] = 2;

    indices[6] = 4; indices[7] = 5; indices[8] = 7;
    indices[9] = 7; indices[10] = 5; indices[11] = 6;

    indices[12] = 8; indices[13] = 9; indices[14] = 11;
    indices[15] = 11; indices[16] = 9; indices[17] = 10;

    indices[18] = 12; indices[19] = 13; indices[20] = 15;
    indices[21] = 15; indices[22] = 13; indices[23] = 14;

    indices[24] = 16; indices[25] = 17; indices[26] = 19;
    indices[27] = 19; indices[28] = 17; indices[29] = 18;

    indices[30] = 20; indices[31] = 21; indices[32] = 23;
    indices[33] = 23; indices[34] = 21; indices[35] = 22;

    RawModel* testModel = ObjLoader::loadObjModel("ux0:/data/assets/stall.obj", loader);
    ModelTexture* texture = new ModelTexture(loader->loadTexture("ux0:/data/assets/stallTexture.png"));
    TexturedModel* model = new TexturedModel(testModel, texture);
    vector3f modelPosition;
    modelPosition.x = 0.0f;
    modelPosition.y = 0.0f;
    modelPosition.z = -25.0f;

    Entity* entity = new Entity(model, modelPosition, 0.0f, 0.0f, 0.0f, 1.0f);

    delete [] vertices;
    delete [] indices;

    bool done = false;
    while(!done)
    {
		ctrl_press = ctrl_peek;
		sceCtrlPeekBufferPositive(0, &ctrl_peek, 1);
		ctrl_press.buttons = ctrl_peek.buttons & ~ctrl_press.buttons;

        // Exit
		if (ctrl_press.buttons & SCE_CTRL_START) done = true;

        entity->increaseRotation(0.0f, 1.0f, 0.0f);
        camera->move(ctrl_peek);

        // Render
        DM->startFrame();
        DM->clearScreen(0.0f, 0.0f, 0.0f, 1.0f);

        staticShader->start(DM->getContext());
        staticShader->loadViewMatrix(DM->getContext(), camera);
        renderer->render(DM->getContext(), entity, staticShader);
        staticShader->stop();
        
        DM->closeFrame();
        DM->swapBuffers();
    }

    delete DM;

    sceKernelExitProcess(0);
    return 0;
}