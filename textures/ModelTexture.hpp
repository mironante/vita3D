#ifndef MODELTEXTURE_HPP
#define MODELTEXTURE_HPP

#include <psp2/gxm.h>

namespace Textures
{
    class ModelTexture
    {
    public:
        ModelTexture(SceGxmTexture texture);

        SceGxmTexture getModelTexture();
    private:
        SceGxmTexture _texture;
    };
}

#endif // MODELTEXTURE_HPP