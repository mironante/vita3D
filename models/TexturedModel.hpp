#ifndef TEXTUREDMODEL_HPP
#define TEXTUREDMODEL_HPP

#include "RawModel.hpp"
#include "../textures/ModelTexture.hpp"

using namespace Textures;

namespace Models
{
    class TexturedModel
    {
    public:
        TexturedModel(RawModel* model, ModelTexture* texture);

        RawModel* getRawModel();
        ModelTexture* getTexture();
    private:
        RawModel* _rawModel;
        ModelTexture* _texture;
    };
}

#endif // TEXTUREDMODEL_HPP