#ifndef COLLIDER_HPP
#define COLLIDER_HPP


#include "Bound.hpp"
#include "Transform.hpp"


class Collider {

private:

    bool        GetSeparatingPlane(const vec3 & RPos, const vec3 Plane, const Bound & bound1, const Bound &bound2);
    bool        GetCollision(const Bound & bound1, const Bound & bound2, const vec3 & RPos);

public:
    ~Collider();
    Collider(bool isTrigger = false);
    Collider(const Collider & rhs, bool isTrigger = false);
    Collider(const Collider & rhs, Transform & transform, bool isTrigger = false);
    Collider(Transform & transform, vec3 minValues, vec3 maxValues, bool isTrigger = false);
    Collider(Transform & transform, vec3 minValues, vec3 maxValues, vec3 offset, bool isTrigger = false);

    vec3        position; //local position (relative to transform.position)
    Transform   *transform;
    bool        isTrigger;
    Bound       bound;

    bool        CheckCollision(Collider & collider);
    bool        CheckCollision(vec3 point); //??
    void        UpdateCollider(); //??
    vec3        GetSize(); //??
    vec3        GetOffsetLocalPosition(vec3 point);
    vec3        GetOffsetWorldPosition();
    Bound       BoundToWorld();
    static Bound    BoundToWorld(Bound & bound);
};

#endif