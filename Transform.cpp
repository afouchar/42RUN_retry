#include "Transform.hpp"


Transform::~Transform(){}

Transform::Transform(){

    this->modelMatrix = mat4(1.0f);
    this->position = vec3(0.0f, 0.0f, 0.0f);
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;
}

Transform::Transform(const Transform& rhs){

    this->position = rhs.position;
    this->rotation = rhs.rotation;
    this->modelMatrix = rhs.modelMatrix;

    this->_horizontalAngle = rhs._horizontalAngle;
    this->_verticalAngle = rhs._verticalAngle;
    this->_direction = rhs._direction;
    this->_right = rhs._right;
    this->_up = rhs._up;
}

Transform::Transform(vec3 pos){

    this->modelMatrix = mat4(1.0f);
    this->position = pos;
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;
}

mat4 Transform::LookAt(vec3 target, vec3 up){
	return glm::lookAt(this->position, target, up);
}

void Transform::Translate(vec3 translation){
    this->position += translation;
    this->modelMatrix = translate(this->modelMatrix, translation);
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

// void Transform::Rotate(vec3 rotation, bool localOrientation){

//     mat4 rotationMatrix = rotate(rotation.x, vec3(1,0,0));
//     rotationMatrix *= rotate(rotation.y, vec3(0,1,0));
//     rotationMatrix *= rotate(rotation.z, vec3(0,0,1));
//     if(localOrientation)
//         this->T = this->T * rotationMatrix;
//     else
//         this->T = rotationMatrix * this->T;
// }

vec3 Transform::Up(){
    return this->_up;
}

vec3 Transform::Right(){
    return this->_right;
}
