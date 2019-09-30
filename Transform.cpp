#include "Transform.hpp"


Transform::~Transform(){}

Transform::Transform(){

    this->position = vec3(0.0f, 0.0f, 0.0f);
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;
}

Transform::Transform(vec3 pos){

    this->position = pos;
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;
}

mat4 Transform::LookAt(vec3 target, vec3 up){
	return glm::lookAt(this->position, target, up);
}

void Transform::UpdateDirection(vec2 mouseDirection){

    // Compute new orientation
	this->_horizontalAngle += mouseDirection.x;
	this->_verticalAngle += mouseDirection.y;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	this->_direction = vec3(cos(this->_verticalAngle) * sin(this->_horizontalAngle), sin(this->_verticalAngle), cos(this->_verticalAngle) * cos(this->_horizontalAngle));
	
	// Right vector
	this->_right = vec3(sin(this->_horizontalAngle - 3.14f/2.0f), 0, cos(this->_horizontalAngle - 3.14f/2.0f));
	
	// Up vector
	this->_up = glm::cross(this->_right, this->_direction);
}

vec3 Transform::GetDirection(){
    return this->_direction;
}

vec3 Transform::Up(){
    return this->_up;
}

vec3 Transform::Right(){
    return this->_right;
}
