#include "Entity.hpp"

namespace Entities
{
    Entity::Entity(TexturedModel* model, vector3f position, float rotX, float rotY, float rotZ, float scale)
    {
        _model = model;
        _position = position;
        _rotX = rotX;
        _rotY = rotY;
        _rotZ = rotZ;
        _scale = scale;
    }

    TexturedModel* Entity::getModel()
    {
        return _model;
    }

    void Entity::setModel(TexturedModel* model)
    {
        _model = model;
    }

    vector3f Entity::getPosition()
    {
        return _position;
    }

    void Entity::setPosition(vector3f position)
    {
        _position = position;
    }

    float Entity::getRotX()
    {
        return _rotX;
    }

    void Entity::setRotX(float rotX)
    {
        _rotX = rotX;
    }

    float Entity::getRotY()
    {
        return _rotY;
    }

    void Entity::setRotY(float rotY)
    {
        _rotY =rotY;
    }

    float Entity::getRotZ()
    {
        return _rotZ;
    }

    void Entity::setRotZ(float rotZ)
    {
        _rotZ = rotZ;
    }

    float Entity::getScale()
    {
        return _scale;
    }

    void Entity::setScale(float scale)
    {
        _scale = scale;
    }

    void Entity::increasePosition(float dx, float dy, float dz)
    {
        _position.x += dx;
        _position.y += dy;
        _position.z += dz;
    }

    void Entity::increaseRotation(float dx, float dy, float dz)
    {
        _rotX += dx;
        _rotY += dy;
        _rotZ += dz;
    }
}
