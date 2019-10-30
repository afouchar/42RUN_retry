#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "Transform.hpp"


using namespace glm;

class Camera {

private:
    mat4        _projection;
    mat4        _view;
    float       _FOV;
    float       _aspectRatio;
    float       _nearPlane;
    float       _farPlane;


public:
    Camera(float FOVdegrees, float aspectRatio, float nearPlane, float farPlane);
    ~Camera();

    Transform   transform;

    mat4        GetProjectionMatrix();
    mat4        GetViewMatrix();

    void        SetProjection(float FOVdegrees, float aspectRatio, float nearPlane, float farPlane);
    void        LookAt(vec3 target, vec3 up);
    void        LookAt(vec3 target);
};

#endif