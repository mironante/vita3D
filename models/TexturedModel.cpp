#include "TexturedModel.hpp"

namespace Models
{

    TexturedModel::TexturedModel(RawModel* model, ModelTexture* texture)
    {
        _rawModel = model;
        _texture = texture;
    }

    RawModel* TexturedModel::getRawModel()
    {
        return _rawModel;
    }

    ModelTexture* TexturedModel::getTexture()
    {
        return _texture;
    }

}