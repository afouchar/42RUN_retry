#ifndef COLLIDER_HPP
#define COLLIDER_HPP
// #pragma once
// #include <list>

#include "Transform.hpp"
#include <vector>

using namespace std;
using namespace glm;


struct Bound {

    vec3            frontLeftDown;
    vec3            frontLeftUp;
    vec3            frontRightUp;
    vec3            frontRightDown;
    vec3            backLeftDown;
    vec3            backLeftUp;
    vec3            backRightUp;
    vec3            backRightDown;

    vec3            up;
    vec3            down;
    vec3            left;
    vec3            right;
    vec3            forward;
    vec3            back;

    vec3            size;
    vec3            min;
    vec3            max;

    vector<vec3 *>  points;
    vector<vec3 *>  normals;

    ~Bound(){}
    Bound(){}
    Bound(vec3 min, vec3 max){

        SetBound(min, max);

        points.push_back(&this->frontLeftDown);
        points.push_back(&this->frontLeftUp);
        points.push_back(&this->frontRightUp);
        points.push_back(&this->frontRightDown);
        points.push_back(&this->backLeftDown);
        points.push_back(&this->backLeftUp);
        points.push_back(&this->backRightUp);
        points.push_back(&this->backRightDown);

        this->up = vec3_up;
        this->down = vec3_down;
        this->left = vec3_left;
        this->right = vec3_right;
        this->forward = vec3_forward;
        this->back = vec3_back;

        normals.push_back(&this->up);
        normals.push_back(&this->down);
        normals.push_back(&this->left);
        normals.push_back(&this->right);
        normals.push_back(&this->forward);
        normals.push_back(&this->back);
    }

    Bound(const Bound & rhs){
        this->frontLeftDown = rhs.frontLeftDown;
        this->frontLeftUp = rhs.frontLeftUp;
        this->frontRightUp = rhs.frontRightUp;
        this->frontRightDown = rhs.frontRightDown;
        this->backLeftDown = rhs.backLeftDown;
        this->backLeftUp = rhs.backLeftUp;
        this->backRightUp = rhs.backRightUp;
        this->backRightDown = rhs.backRightDown;

        this->up = rhs.up;
        this->down = rhs.down;
        this->left = rhs.left;
        this->right = rhs.right;
        this->forward = rhs.forward;
        this->back = rhs.back;

        this->size = rhs.size;
        this->min = rhs.min;
        this->max = rhs.max;

        points.push_back(&this->frontLeftDown);
        points.push_back(&this->frontLeftUp);
        points.push_back(&this->frontRightUp);
        points.push_back(&this->frontRightDown);
        points.push_back(&this->backLeftDown);
        points.push_back(&this->backLeftUp);
        points.push_back(&this->backRightUp);
        points.push_back(&this->backRightDown);

        normals.push_back(&this->up);
        normals.push_back(&this->down);
        normals.push_back(&this->left);
        normals.push_back(&this->right);
        normals.push_back(&this->forward);
        normals.push_back(&this->back);
    }

    void SetBound(vec3 min, vec3 max){

        this->min = min;
        this->max = max;

        this->size.x = glm::distance(this->max.x, this->min.x);
        this->size.y = glm::distance(this->max.y, this->min.y);
        this->size.z = glm::distance(this->max.z, this->min.z);

        this->backLeftDown = min;
        this->backLeftUp = this->backLeftDown + (this->size * vec3_up);
        this->backRightDown = this->backLeftDown + (this->size * vec3_right);
        this->backRightUp = this->backLeftUp + (this->size * vec3_right);

        this->frontRightUp = max;
        this->frontRightDown = this->frontRightUp + (this->size * vec3_down);
        this->frontLeftUp = this->frontRightUp + (this->size * vec3_left);
        this->frontLeftDown = this->frontRightDown + (this->size * vec3_left);
    }
};

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

    vec3        position; //local position (relative to transform)
    // vec3        max;
    // vec3        min;
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
    static Bound    BoundToWorld(Bound & bound, Transform & transform);

    // static void UpdateCollisions();

    //derived virtual methods
    // virtual void    OnColliderEnter(Collider & collider){};
    // virtual void    OnColliderStay(Collider & collider){};
    // virtual void    OnColliderExit(Collider & collider){};

};

#endif
