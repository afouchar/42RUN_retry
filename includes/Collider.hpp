#ifndef COLLIDER_HPP
#define COLLIDER_HPP
// #pragma once
// #include <list>

#include "Transform.hpp"

using namespace std;
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

    // static void UpdateCollisions();

    //derived virtual methods
    // virtual void    OnColliderEnter(Collider & collider){};
    // virtual void    OnColliderStay(Collider & collider){};
    // virtual void    OnColliderExit(Collider & collider){};

};

#endif
