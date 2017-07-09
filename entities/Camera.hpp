#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../toolbox/math_utils.h"
#include <psp2/ctrl.h>

namespace Entities
{
    class Camera
    {
    public:
        Camera();

        vector3f getPosition();
        float getPitch();
        float getYaw();
        float getRoll();

        void move(SceCtrlData ctrl);
    private:
        vector3f _position;
        float _pitch;
        float _yaw;
        float _roll;
    };
}

#endif // CAMERA_HPP