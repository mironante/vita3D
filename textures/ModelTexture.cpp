#include "ModelTexture.hpp"

namespace Textures
{
    ModelTexture::ModelTexture(SceGxmTexture texture)
    {
        _texture = texture;
    }

    SceGxmTexture ModelTexture::getModelTexture()
    {
        return _texture;
    }

    float ModelTexture::getShineDamper()
    {
        return _shineDamper;
    }

    void ModelTexture::setShineDamper(float shineDamper)
    {
        _shineDamper = shineDamper;
    }

    float ModelTexture::getReflectivity()
    {
        return _reflectivity;
    }

    void ModelTexture::setReflectivity(float reflectivity)
    {
        _reflectivity = reflectivity;
    }
}
