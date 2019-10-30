#include "Camera.hpp"
#include "RenderPipeline.hpp"
// #include <iostream>

Camera::~Camera(){
    RenderPipeline::RemoveCamera((*this));
}

Camera::Camera(float FOVdegrees, float aspectRatio, float nearPlane, float farPlane){

    this->_FOV = FOVdegrees;
    this->_aspectRatio = aspectRatio;
    this->_nearPlane = nearPlane;
    this->_farPlane = farPlane;
    this->_projection = perspective(radians(this->_FOV), this->_aspectRatio, this->_nearPlane, this->_farPlane);
    this->_view = this->transform.LookAt(vec3_zero, vec3_up);
    RenderPipeline::AddCamera((*this));
}

void Camera::SetProjection(float FOVdegrees, float aspectRatio, float nearPlane, float farPlane){

    this->_projection = perspective(radians(FOVdegrees), aspectRatio, nearPlane, farPlane);
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

