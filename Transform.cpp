#include "Transform.hpp"


Transform::~Transform(){}

Transform::Transform(){

    this->position = vec3(0.0f, 0.0f, 0.0f);
}

Transform::Transform(vec3 pos){

    this->position = pos;
}

mat4 Transform::LookAt(vec3 target, vec3 up){
	return glm::lookAt(this->position, target, up);
}