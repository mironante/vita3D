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
#include "entities/Light.hpp"


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
    StaticShader* shader = new StaticShader(DM->getShaderPatcher());

    RawModel* model = ObjLoader::loadObjModel("ux0:/data/assets/dragon.obj", loader);
    ModelTexture* texture = new ModelTexture(loader->loadTexture("ux0:/data/assets/dragon.png"));
    texture->setShineDamper(10);
    texture->setReflectivity(1);
    TexturedModel* staticModel = new TexturedModel(model, texture);

    // Entity
    vector3f modelPosition;
    modelPosition.x = 0.0f;
    modelPosition.y = 0.0f;
    modelPosition.z = -25.0f;
    Entity* entity = new Entity(staticModel, modelPosition, 0.0f, 0.0f, 0.0f, 1.0f);
    
    // Light
    vector3f lightPosition;
    lightPosition.x = 200.0f;
    lightPosition.y = 200.0f;
    lightPosition.z = 100.0f;
    vector3f lightColor;
    lightColor.r = 1.0f;
    lightColor.g = 1.0f;
    lightColor.b = 1.0f;
    Light* light = new Light(lightPosition, lightColor);

    Camera* camera = new Camera();

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

        shader->start(DM->getContext());
        shader->loadLight(DM->getContext(), light);
        shader->loadViewMatrix(DM->getContext(), camera);
        renderer->render(DM->getContext(), entity, shader);
        shader->stop();
        
        DM->closeFrame();
        DM->swapBuffers();
    }

    delete DM;

    sceKernelExitProcess(0);
    return 0;
}