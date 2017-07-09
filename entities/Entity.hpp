#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../models/TexturedModel.hpp"
#include "../toolbox/math_utils.h"

using namespace Models;

namespace Entities
{
    class Entity
    {
    public:
        Entity(TexturedModel* model, vector3f position, float rotX, float rotY, float rotZ, float scale);

        TexturedModel* getModel();
        void setModel(TexturedModel* model);

        vector3f getPosition();
        void setPosition(vector3f position);

        float getRotX();
        void setRotX(float rotX);

        float getRotY();
        void setRotY(float rotY);

        float getRotZ();
        void setRotZ(float rotZ);

        float getScale();
        void setScale(float scale);

        void increasePosition(float dx, float dy, float dz);
        void increaseRotation(float dx, float dy, float dz);
    private:
        TexturedModel* _model;
        vector3f _position;
        float _rotX;
        float _rotY;
        float _rotZ;
        float _scale;
    };


};

#endif // ENTITY_HPP