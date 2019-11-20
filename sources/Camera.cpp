#include "Camera.hpp"
#include "RenderPipeline.hpp"
#include "GameBehaviour.hpp"
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
    this->target = nullptr;
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

void Camera::Follow(Transform & target, vec3 offset){

    vec3 pos = this->transform.position;
    pos.x = target.position.x + offset.x;
    pos.y = target.position.y + offset.y;
    pos.z += offset.z;
    this->transform.position = pos;
}

void Camera::SmoothFollow(Transform & target, vec3 offset, float speed){

    vec3 pos = this->transform.position;
    pos.x = target.position.x + offset.x;
    pos.y = target.position.y + offset.y;
    pos.z += offset.z;

    this->transform.position = glm::mix(this->transform.position, pos, speed);
}

void Camera::SetTarget(Transform & target){
    this->target = &target;
}


void Camera::LateUpdate(){

    if (this->target == nullptr)
        return;

    vec3 offsetTarget = (this->target->Up() * 8.0f) + (this->target->Forward() * 50.0f);
    SmoothFollow((*this->target), this->target->Up() * 6.0f, GameBehaviour::DeltaTime() * 3.0f);
    LookAt(this->target->position + offsetTarget, this->target->Up());
}