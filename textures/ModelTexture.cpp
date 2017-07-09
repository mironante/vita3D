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
}
