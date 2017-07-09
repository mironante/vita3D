#ifndef DISPLAYMANAGER_HPP
#define DISPLAYMANAGER_HPP

#include <psp2/gxm.h>
#include <psp2/display.h>

#define DISPLAY_WIDTH 960
#define DISPLAY_HEIGHT 544
#define DISPLAY_STRIDE 1024

#define DISPLAY_BUFFER_COUNT 2
#define MAX_PENDING_SWAPS (DISPLAY_BUFFER_COUNT - 1)

#define DISPLAY_COLOR_FORMAT SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT SCE_DISPLAY_PIXELFORMAT_A8B8G8R8

namespace RenderEngine
{
    typedef struct DisplayData 
    {
        void *address;
    } DisplayData;

    class DisplayManager
    {
    public:
        DisplayManager();
        ~DisplayManager();

        void startFrame();

        void closeFrame();

        void swapBuffers();

        SceGxmContext* getContext();
        SceGxmShaderPatcher* getShaderPatcher();

        void clearScreen(float r, float g, float b, float a);

    private:

    };
}

#endif // DISPLAYMANAGER_HPP