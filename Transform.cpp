#include "Transform.hpp"


Transform::~Transform(){}

Transform::Transform(){

    this->modelMatrix = mat4(1.0f);
    this->position = vec3(0.0f, 0.0f, 0.0f);
    this->rotation = vec3(0.0f, 0.0f, 0.0f);
    this->scale = vec3(1.0f, 1.0f, 1.0f);
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;

    this->_rotation = mat4(1.0f);
    this->_translation = mat4(1.0f);
    this->_scale = mat4(1.0f);
}

Transform::Transform(const Transform& rhs){

    this->position = rhs.position;
    this->rotation = rhs.rotation;
    this->scale = rhs.scale;
    this->modelMatrix = rhs.modelMatrix;

    this->_horizontalAngle = rhs._horizontalAngle;
    this->_verticalAngle = rhs._verticalAngle;
    this->_direction = rhs._direction;
    this->_right = rhs._right;
    this->_up = rhs._up;

    this->_rotation = rhs._rotation;
    this->_translation = rhs._translation;
    this->_scale = rhs._scale;
}

Transform::Transform(vec3 pos){

    this->modelMatrix = mat4(1.0f);
    this->position = pos; //translate??
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;
}

mat4 Transform::LookAt(vec3 target, vec3 up){
	return glm::lookAt(this->position, target, up);
}

// void Transform::Translate(vec3 translation){
//     this->position += translation;
//     this->modelMatrix = translate(this->modelMatrix, translation);
// }

// void Transform::Rotate(vec3 anglesDegrees){

//     vec3 anglesRadians = radians(anglesDegrees);

//     this->modelMatrix = rotate(this->modelMatrix, anglesRadians.z, vec3_forward);
// }


void Transform::Translate(const vec3 &axis){

    this->position += axis;
    this->_translation = glm::translate(this->_translation, axis); 
    UpdateMatrix();
}

void Transform::Rotate(vec3 axis, float angleDegrees){
    this->rotation += (axis * angleDegrees);
    this->_rotation = glm::rotate(this->_rotation, radians(angleDegrees), axis);
    UpdateMatrix();
}

#include <iostream>

void Transform::Scale(vec3 axis){
    this->scale += axis;
    std::cout << "scale : x[" << this->scale.x << "] y[" << this->scale.y << "] z[" << this->scale.z << "]" << std::endl;
    this->_scale = glm::scale(this->_scale, this->scale);
    UpdateMatrix();
}

void Transform::RotateAround(vec3 pivot){

    mat4 translate = glm::translate(mat4(), pivot);
    mat4 invTranslate = glm::inverse(translate);

  // The idea:
  // 1) Translate the object to the center
  // 2) Make the rotation
  // 3) Translate the object back to its original location

  mat4 transform = translate * this->_rotation * invTranslate;
  this->position = transform * vec4(pivot, 1.0f);
  glm::translate(this->_translation, this->position);
  UpdateMatrix();
}

void Transform::UpdateMatrix(){
    this->modelMatrix = this->_scale * this->_translation * this->_rotation ;
}

void Transform::ResetMatrix() {

    this->modelMatrix = mat4(1.0f); 
    this->_rotation = mat4(1.0f);
    this->_scale = mat4(1.0f);
    this->_translation = mat4(1.0f);
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
