#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/quaternion.hpp>
#include <string>
#include <list>


#define vec3_zero       vec3(0, 0, 0)
#define vec3_one        vec3(1, 1, 1)
#define vec3_left       vec3(-1, 0, 0)
#define vec3_right      vec3(1, 0, 0)
#define vec3_up         vec3(0, 1, 0)
#define vec3_down       vec3(0, -1, 0)
#define vec3_forward    vec3(0, 0, -1)
#define vec3_back       vec3(0, 0, 1)
#define PI              3.14159265358979323846

class Object;

using namespace glm;
using namespace std;


class Transform {
private:

    float               _horizontalAngle;
    float               _verticalAngle;
    vec3                _direction;
    vec3                _right;
    vec3                _up;

    mat4                _matRotation;
    mat4                _matTranslation;
    mat4                _matScale;
    quat                _quatRotation;
    string              _tag;

    void                UpdateMatrixFromRoot();


public:
    Transform();
    Transform(Object & gameObject);
    Transform(const Transform& rhs);
    Transform(const Transform& rhs, Object & gameObject);
    Transform(vec3 pos);
    Transform(vec3 pos, Object & gameObject);
    ~Transform();

    list<Transform *>   child;
    Transform           *parent;
    Object              *gameObject;

    vec3                position;
    vec3                eulerAngles;
    vec3                scale;
    mat4                modelMatrix;

    void                AddChild(Transform & child);
    void                RemoveChild(Transform & child);
    void                AddParent(Transform & parent);
    void                RemoveParent();
    void                ClearParenting();
    void                ClearParenting(Transform & newParent);
    mat4                LookAt(vec3 target, vec3 up);
    void                Translate(const vec3 &axis);
    void                Rotate(vec3 axis, float angleDegrees);
    void                Scale(vec3 axis);
    void                RotateAround(vec3 pivot, vec3 axis, float angleDegrees);
    static vec3         RotatePointAround(vec3 pivot, vec3 point, vec3 axis, float angleDegrees);
    void                SetRotation(quat newRotation);
    void                Interpolate(quat targetRot, float angle);
    static float        Distance(vec3 start, vec3 end);
    void                UpdateMatrix();
    void                ResetMatrix();
    void                Reset();
    vec3                WorldPosition();
    void                LocalToWorld();

    void                UpdateDirection(vec2 mouseDirection);
    vec3                GetDirection();

    vec3                Up();
    vec3                Down();
    vec3                Left();
    vec3                Right();
    vec3                Forward();
    vec3                Back();
    string              GetTag();
    void                SetTag(string newTag);
    Transform           *GetRoot();
    quat                GetQuaternion();

    bool                operator == (const Transform & rhs) const {
        return &rhs == &(*this);
    }
};

#endif