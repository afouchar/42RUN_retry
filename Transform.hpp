#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/quaternion.hpp>
// #include <gtc/quaternion.hpp>

#include <string>

#define vec3_zero       vec3(0, 0, 0)
#define vec3_one        vec3(1, 1, 1)
#define vec3_left       vec3(-1, 0, 0)
#define vec3_right      vec3(1, 0, 0)
#define vec3_up         vec3(0, 1, 0)
#define vec3_down       vec3(0, -1, 0)
#define vec3_forward    vec3(0, 0, -1)
#define vec3_back       vec3(0, 0, 1)
#define PI              3.14159265358979323846

using namespace glm;
using namespace std;


class Transform {
private:

    float       _horizontalAngle;
    float       _verticalAngle;
    vec3        _direction;
    vec3        _right;
    vec3        _up;

    // mat4        model;
    mat4        _rotation;
    mat4        _translation;
    mat4        _scale;
    quat        _quatRotation;
    string      _tag;



public:
    Transform();
    Transform(const Transform& rhs);
    Transform(vec3 pos);
    ~Transform();

    Transform   *child;
    Transform   *parent;
    
    vec3        position;
    vec3        rotation;
    vec3        scale;
    mat4        modelMatrix;

    mat4        LookAt(vec3 target, vec3 up);
    void        Translate(const vec3 &axis);
    void        Rotate(vec3 axis, float angleDegrees);
    void        Scale(vec3 axis);
    void        RotateAround(vec3 pivot, vec3 axis, float angleDegrees);
    void        UpdateMatrix();
    void        ResetMatrix();
    vec3        LocalToWorldPosition();
    vec3        LocalToWorldRotation();

    void        UpdateDirection(vec2 mouseDirection);
    vec3        GetDirection();

    quat        RotateTowards(quat q1, quat q2, float maxAngle);

    vec3        Up();
    vec3        Right();
    vec3        Forward();
    string      GetTag();
    void        SetTag(string newTag);
    Transform   *GetRoot();
};

#endif