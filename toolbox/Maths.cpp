#include "Maths.hpp"
#include <string.h>
#include <stdlib.h>

namespace Toolbox
{
    float* Maths::createTransformationMatrix(vector3f translation, float rx, float ry, float rz, float scale)
    {
        matrix4x4 matrix;
        matrix4x4 mt, mrx, mry, mrz, ms;
        matrix4x4 mtmp1, mtmp2, mtmp3;

        matrix4x4_init_translation_vector3f(mt, &translation);
        matrix4x4_init_rotation_x(mrx, DEG_TO_RAD(rx));
        matrix4x4_init_rotation_y(mry, DEG_TO_RAD(ry));
        matrix4x4_init_rotation_z(mrz, DEG_TO_RAD(rz));
        matrix4x4_init_scaling(ms, scale, scale, scale);

        matrix4x4_multiply(mtmp1, mrx, mry);
        matrix4x4_multiply(mtmp2, mrz, mtmp1);
        matrix4x4_multiply(mtmp3, mt, mtmp2);

        matrix4x4_multiply(matrix, mtmp3, ms);

        float* result = new float[16];
        memcpy(result, matrix, 16 * sizeof(float));

        return result;
    }

    float* Maths::createViewMatrix(Camera* camera)
    {
        matrix4x4 matrix;
        matrix4x4 mt, mrx, mry;
        matrix4x4 mtmp1;
        matrix4x4_init_rotation_x(mrx, DEG_TO_RAD(camera->getPitch()));
        matrix4x4_init_rotation_y(mry, DEG_TO_RAD(camera->getYaw()));     
        matrix4x4_multiply(mtmp1, mrx, mry);

        vector3f cameraPos = camera->getPosition();
        vector3f negativeCameraPos;
        negativeCameraPos.x = -cameraPos.x;
        negativeCameraPos.y = -cameraPos.y;
        negativeCameraPos.z = -cameraPos.z;
        matrix4x4_init_translation_vector3f(mt, &negativeCameraPos);
        matrix4x4_multiply(matrix, mt, mtmp1);

        float* result = new float[16];
        memcpy(result, matrix, 16 * sizeof(float));

        return result;       
    }
}