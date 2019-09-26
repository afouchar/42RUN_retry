#include "Camera.hpp"


Camera::~Camera(){}

Camera::Camera(float FOVdegrees, float aspectRatio, float nearPlane, float farPlane){

    this->_FOV = radians(FOVdegrees);
    this->_aspectRatio = aspectRatio;
    this->_nearPlane = nearPlane;
    this->_farPlane = farPlane;
    SetProjection(this->_FOV, this->_aspectRatio, this->_nearPlane, this->_farPlane);
    this->_view = this->transform.LookAt(vec3_zero, vec3_up);
}

void Camera::SetProjection(float FOVdegrees, float aspectRatio, float nearPlane, float farPlane){

    this->_projection = perspective(FOVdegrees, aspectRatio, nearPlane, farPlane);
}

void Camera::LookAt(vec3 target, vec3 up){
    this->_view = this->transform.LookAt(target, up);
}

void Camera::LookAt(vec3 target){
    this->_view = this->transform.LookAt(target, vec3_up);
}

mat4 Camera::GetProjectionMatrix(){
    return this->_projection;
}

mat4 Camera::GetViewMatrix(){
    return this->_view;    
}

