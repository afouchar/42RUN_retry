#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "ScriptableBehaviour.hpp"
#include "Transform.hpp"


using namespace glm;

class Camera : public ScriptableBehaviour {

private:
    mat4            _projection;
    mat4            _view;
    float           _FOV;
    float           _aspectRatio;
    float           _nearPlane;
    float           _farPlane;


public:
    Camera(float FOVdegrees, float aspectRatio, float nearPlane, float farPlane);
    ~Camera();

    Transform       transform;
    Transform       *target;

    mat4            GetProjectionMatrix();
    mat4            GetViewMatrix();

    void            SetProjection(float FOVdegrees, float aspectRatio, float nearPlane, float farPlane);
    void            LookAt(vec3 target, vec3 up);
    void            LookAt(vec3 target);
    void            Follow(Transform & target, vec3 offset);
    void            SmoothFollow(Transform & target, vec3 offset, float speed);
    void            SetTarget(Transform & target);

    //virtual methods
    virtual void        OnColliderEnter(Collider & collider){};
    virtual void        OnColliderStay(Collider & collider){};
    virtual void        OnColliderExit(Collider & collider){};
    virtual void        Update(){};
    virtual void        LateUpdate();
    virtual Collider    *GetCollider(){ return nullptr; };
};

#endif