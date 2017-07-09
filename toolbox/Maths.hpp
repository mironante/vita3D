#ifndef MATHS_HPP
#define MATHS_HPP

#include "math_utils.h"
#include "../entities/Camera.hpp"

using namespace Entities;

namespace Toolbox
{
    class Maths
    {
    public:
        Maths() {};

        float* createTransformationMatrix(vector3f translation, float rx, float ry, float rz, float scale);
        float* createViewMatrix(Camera* camera);
    private:
    };
}

#endif // MATHS_HPP