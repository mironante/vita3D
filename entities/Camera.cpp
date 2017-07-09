#include "Camera.hpp"

namespace Entities
{
    Camera::Camera()
    {
        vector3f_init(&_position, 0.0f, 0.0f, 0.0f);
    }

    vector3f Camera::getPosition()
    {
        return _position;
    }

    float Camera::getPitch()
    {
        return _pitch;
    }

    float Camera::getYaw()
    {
        return _yaw;
    }

    float Camera::getRoll()
    {
        return _roll;
    }

    void Camera::move(SceCtrlData ctrl)
    {
        if (ctrl.buttons & SCE_CTRL_UP)
        {
            _position.z -= 0.02f;
        }

        if (ctrl.buttons & SCE_CTRL_RIGHT)
        {
            _position.x += 0.02f;
        }

        if (ctrl.buttons & SCE_CTRL_LEFT)
        {
            _position.x -= 0.02f;
        }
    }
}