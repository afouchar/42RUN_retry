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
    this->_quatRotation = toQuat(this->_rotation);

    this->child = nullptr;
    this->parent = nullptr;
    this->_tag = "";
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
    this->_quatRotation = rhs._quatRotation;

    this->parent = nullptr;
    this->child = nullptr;
    this->_tag = rhs._tag;
}

Transform::Transform(vec3 pos){

    this->modelMatrix = mat4(1.0f);
    this->position = pos;
    this->rotation = vec3(0.0f, 0.0f, 0.0f);
    this->scale = vec3(1.0f, 1.0f, 1.0f);
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;

    this->_rotation = mat4(1.0f);
    this->_translation = mat4(1.0f);
    this->_scale = mat4(1.0f);
    this->_quatRotation = toQuat(this->_rotation);

    this->child = nullptr;
    this->parent = nullptr;
    this->_tag = "";
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

void Transform::UpdateTranslate(){
    this->_translation = glm::translate(this->_translation, this->position);
    if (this->child != nullptr)
        this->child->UpdateTranslate();
}

void Transform::Translate(const vec3 &axis){

    this->position += axis;
    this->_translation = glm::translate(this->_translation, axis); 
    GetRoot()->UpdateMatrix();

    // if (this->child != nullptr)
        // this->child->Translate(axis); //relative to parent??
}

void Transform::Rotate(vec3 axis, float angleDegrees){
    axis = normalize(axis);
    this->rotation += (axis * angleDegrees);

    // this->_quatRotation *= angleAxis(radians(angleDegrees), axis);
    this->_quatRotation *= quat(axis * radians(angleDegrees));
    // this->_rotation = glm::rotate(this->_rotation, radians(angleDegrees), axis);
    // this->_quatRotation = quat(vec3(radians(this->rotation.x), radians(this->rotation.y), radians(this->rotation.z)));
    GetRoot()->UpdateMatrix();
    // if (this->child != nullptr)
    //     this->child->Rotate(axis, angleDegrees); //relative to parent??
}

void Transform::Scale(vec3 axis){
    this->scale += axis;
    // std::cout << "scale : x[" << this->scale.x << "] y[" << this->scale.y << "] z[" << this->scale.z << "]" << std::endl;
    this->_scale = glm::scale(this->_scale, axis);
    GetRoot()->UpdateMatrix();

    // if (this->_child != nullptr)
    //     this->_child->Scale(axis); //relative to parent??
}

void Transform::RotateAround(vec3 pivot, vec3 axis, float angleDegrees){

// rotated_point = origin + (orientation_quaternion * (point-origin));
    // axis = normalize(axis);
    // this->rotation += (axis * angleDegrees);
    // quat rotAround = angleAxis(radians(angleDegrees), axis);
    quat rotAround = quat(axis * radians(angleDegrees));
    // this->_rotation = toMat4(this->_quatRotation);
    this->position = this->position + (rotAround * (pivot - this->position));

//     mat4 translate = glm::translate(mat4(), pivot);
//     mat4 invTranslate = glm::inverse(translate);

//   // The idea:
//   // 1) Translate the object to the center
//   // 2) Make the rotation
//   // 3) Translate the object back to its original location

//   mat4 transform = translate * this->_rotation * invTranslate;
//   this->position = transform * vec4(pivot, 1.0f);
//   glm::translate(this->_translation, this->position);
//   GetRoot()->UpdateMatrix();
}

void Transform::UpdateMatrix(){


    mat4 parentModelMatrix = mat4(1.0f);
    if (this->parent != nullptr)
        parentModelMatrix = this->parent->modelMatrix;

        

    this->_translation = glm::translate(mat4(1.0f), this->position);
    this->_scale = glm::scale(mat4(1.0f), this->scale);
    // this->_rotation = glm::rotate(this->_rotation, 0.0f, vec3_up);
    this->_rotation = toMat4(this->_quatRotation);

    this->modelMatrix = parentModelMatrix * (this->_scale * this->_translation * this->_rotation);
    // this->modelMatrix = parentModelMatrix * this->_translation;

    if (this->child != nullptr)
        this->child->UpdateMatrix();
}

void Transform::ResetMatrix() {

    this->modelMatrix = mat4(1.0f); 
    this->_rotation = mat4(1.0f);
    this->_scale = mat4(1.0f);
    this->_translation = mat4(1.0f);
}

vec3 Transform::LocalToWorldPosition(){

    return vec3(this->modelMatrix[3]);

    // // mat4 d_transformation;
    // vec3 d_scale;
    // quat d_rotation;
    // vec3 d_translation;
    // vec3 d_skew;
    // vec4 d_perspective;
    // glm::decompose(this->modelMatrix, d_scale, d_rotation, d_translation, d_skew, d_perspective);
    // d_rotation = glm::conjugate(d_rotation);
    // return d_translation;
}

vec3 Transform::LocalToWorldRotation(){

    vec3 d_scale;
    quat d_rotation;
    vec3 d_translation;
    vec3 d_skew;
    vec4 d_perspective;
    glm::decompose(this->modelMatrix, d_scale, d_rotation, d_translation, d_skew, d_perspective);
    d_rotation = glm::conjugate(d_rotation);

    this->_quatRotation = d_rotation;
    this->_rotation = toMat4(this->_quatRotation);
    this->rotation = vec3(glm::degrees(this->_rotation[2][0]), glm::degrees(this->_rotation[2][1]), glm::degrees(this->_rotation[2][2]));
    return this->rotation;
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
    // return this->_up;
    mat4 inverted = glm::inverse(this->modelMatrix);
    vec3 up = glm::normalize(glm::vec3(inverted[1]));
    return up;
}

vec3 Transform::Right(){
    // return this->_right;
    mat4 inverted = glm::inverse(this->modelMatrix);
    vec3 right = glm::normalize(glm::vec3(inverted[0]));
    return right;
}

vec3 Transform::Forward(){
    mat4 inverted = glm::inverse(this->modelMatrix);
    vec3 forward = glm::normalize(glm::vec3(inverted[2]));
    return forward * vec3(1, 1, -1);
}

string Transform::GetTag(){
    return this->_tag;
}

void Transform::SetTag(string newTag){
    this->_tag = newTag;
}

Transform *Transform::GetRoot(){
    if (this->parent != nullptr)
        return this->parent->GetRoot();
    return this;
}

// void Transform::SetChild(Transform *child){
//     this->_child = child;

//     if (this->_child != nullptr && this->_child->parent != this)
//         this->_child->SetParent(this);
// }

// void Transform::SetParent(Transform *parent){

//     this->parent = parent;


//     if (this->parent == nullptr)
//         this->position = LocalToWorldPosition();

//     if (this->parent != nullptr && this->parent->_child != this)
//         this->parent->SetChild(this);
// }