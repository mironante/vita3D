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

        float getShineDamper();
        void setShineDamper(float shineDamper);

        float getReflectivity();
        void setReflectivity(float reflectivity);

    private:
        SceGxmTexture _texture;
        float _shineDamper;
        float _reflectivity;
    };
}

#endif // MODELTEXTURE_HPP