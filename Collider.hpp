#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "Transform.hpp"

using namespace glm;

class Collider {

private:

    vec3        _size;


public:
    ~Collider();
    Collider();
    Collider(const Collider & rhs);
    Collider(const Collider & rhs, Transform & transform);
    Collider(Transform & transform, vec3 minValues, vec3 maxValues);
    Collider(Transform & transform, vec3 minValues, vec3 maxValues, vec3 offset);

    vec3        position; //local position (relative to transform)
    vec3        max;
    vec3        min;
    Transform   *transform;

    bool        CheckCollision(Collider collider);
    bool        CheckCollision(vec3 point);
    void        UpdateCollider();
    vec3        GetSize();
    vec3        GetWorldPosition();
};

#endif