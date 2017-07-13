#include "Light.hpp"

namespace Entities
{
    Light::Light(vector3f position, vector3f color)
    {
        _position = position;
        _color = color;
    }

    vector3f Light::getPosition()
    {
        return _position;
    }

    void Light::setPosition(vector3f position)
    {
        _position = position;
    }

    vector3f Light::getColor()
    {
        return _color;
    }

    void Light::setColor(vector3f color)
    {
        _color = color;
    }
}