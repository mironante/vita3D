#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../toolbox/math_utils.h"

namespace Entities
{
    class Light
    {
    public:
        Light(vector3f position, vector3f color);

        vector3f getPosition();
        void setPosition(vector3f position);

        vector3f getColor();
        void setColor(vector3f color);
    private:
        vector3f _position;
        vector3f _color;
    };
}

#endif // LIGHT_HPP