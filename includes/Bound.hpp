#ifndef BOUND_HPP
#define BOUND_HPP

#include <vector>
#include "Transform.hpp"

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
    Transform       *transform;

    ~Bound();
    Bound();
    Bound(vec3 min, vec3 max, Transform & transform);
    Bound(const Bound & rhs);

    void            SetBound(vec3 min, vec3 max);
};

#endif