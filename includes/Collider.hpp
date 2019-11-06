#ifndef COLLIDER_HPP
#define COLLIDER_HPP


#include "Bound.hpp"
#include "Transform.hpp"


class Collider {

private:

    bool        Intersects(Bound bound1, Bound bound2);
    bool        OverlapTest(Bound bound1, Bound bound2);
    void        SATTest(const vec3 & axis, const vector<vec3 *> & pointsSet, float & minAlong, float & maxAlong);
    bool        Overlaps(float min1, float max1, float min2, float max2);
    bool        IsBetweenOrdered(float val, float lowerBound, float upperBound);

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
    bool        CheckCollision(vec3 point);
    void        UpdateCollider();
    vec3        GetSize();
    vec3        GetOffsetLocalPosition(vec3 point);
    vec3        GetOffsetWorldPosition();
    Bound       BoundToWorld();
    static Bound    BoundToWorld(Bound & bound);
};

#endif