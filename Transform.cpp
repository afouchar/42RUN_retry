#include "Transform.hpp"


Transform::~Transform(){}

Transform::Transform(){

    this->modelMatrix = mat4(1.0f);
    this->position = vec3(0.0f, 0.0f, 0.0f);
    this->eulerAngles = vec3(0.0f, 0.0f, 0.0f);
    this->scale = vec3(1.0f, 1.0f, 1.0f);
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;

    this->_matRotation = mat4(1.0f);
    this->_matTranslation = mat4(1.0f);
    this->_matScale = mat4(1.0f);
    this->_quatRotation = toQuat(this->_matRotation);

    this->child = nullptr;
    this->parent = nullptr;
    this->_tag = "";
}

Transform::Transform(const Transform& rhs){

    this->position = rhs.position;
    this->eulerAngles = rhs.eulerAngles;
    this->scale = rhs.scale;
    this->modelMatrix = rhs.modelMatrix;

    this->_horizontalAngle = rhs._horizontalAngle;
    this->_verticalAngle = rhs._verticalAngle;
    this->_direction = rhs._direction;
    this->_right = rhs._right;
    this->_up = rhs._up;

    this->_matRotation = rhs._matRotation;
    this->_matTranslation = rhs._matTranslation;
    this->_matScale = rhs._matScale;
    this->_quatRotation = rhs._quatRotation;

    this->parent = nullptr;
    this->child = nullptr;
    this->_tag = rhs._tag;
}

Transform::Transform(vec3 pos){

    this->modelMatrix = mat4(1.0f);
    this->position = pos;
    this->eulerAngles = vec3(0.0f, 0.0f, 0.0f);
    this->scale = vec3(1.0f, 1.0f, 1.0f);
    this->_horizontalAngle = PI;
    this->_verticalAngle = 0.0f;

    this->_matRotation = mat4(1.0f);
    this->_matTranslation = mat4(1.0f);
    this->_matScale = mat4(1.0f);
    this->_quatRotation = toQuat(this->_matRotation);

    this->child = nullptr;
    this->parent = nullptr;
    this->_tag = "";
}

mat4 Transform::LookAt(vec3 target, vec3 up){
	return glm::lookAt(this->position, target, up);
}

void Transform::Translate(const vec3 &axis){

    this->position += axis;
}

void Transform::Rotate(vec3 axis, float angleDegrees){
    axis = normalize(axis);
    this->eulerAngles += (axis * angleDegrees);
    vec3 eulerAnglesRadians = vec3(radians(this->eulerAngles.x), radians(this->eulerAngles.y), radians(this->eulerAngles.z));
    this->_quatRotation = quat(eulerAnglesRadians);
}

void Transform::Scale(vec3 axis){
    this->scale += axis;
}

void Transform::RotateAround(vec3 pivot, vec3 axis, float angleDegrees){

    axis = normalize(axis);
    this->eulerAngles += (axis * angleDegrees);
    vec3 eulerAnglesRadians = vec3(radians(this->eulerAngles.x), radians(this->eulerAngles.y), radians(this->eulerAngles.z));
    this->_quatRotation = quat(eulerAnglesRadians);

    this->position = pivot + (this->_quatRotation * (this->position - pivot));    
}

void Transform::Interpolate(quat targetRot, float angle){
    this->_quatRotation = mix(this->_quatRotation, targetRot, angle);
}

void Transform::UpdateMatrix(){

    GetRoot()->UpdateMatrixFromRoot();
}

void Transform::UpdateMatrixFromRoot(){

    mat4 parentModelMatrix = mat4(1.0f);
    if (this->parent != nullptr)
        parentModelMatrix = this->parent->modelMatrix;

    this->_matScale = glm::scale(mat4(1.0f), this->scale);
    this->_matRotation = toMat4(this->_quatRotation);
    // this->position = this->_quatRotation * this->position;
    this->_matTranslation = glm::translate(mat4(1.0f), this->position);

    this->modelMatrix = parentModelMatrix * (this->_matScale * this->_matTranslation * this->_matRotation);

    if (this->child != nullptr)
        this->child->UpdateMatrixFromRoot();
}

void Transform::ResetMatrix() {

    this->modelMatrix = mat4(1.0f); 
    this->_matRotation = mat4(1.0f);
    this->_matScale = mat4(1.0f);
    this->_matTranslation = mat4(1.0f);
}

vec3 Transform::LocalToWorldPosition(){

    return vec3(this->modelMatrix[3]);
}

vec3 Transform::LocalToWorldRotation(){

    vec3 d_matScale;
    quat d_matRotation;
    vec3 d_matTranslation;
    vec3 d_skew;
    vec4 d_perspective;
    glm::decompose(this->modelMatrix, d_matScale, d_matRotation, d_matTranslation, d_skew, d_perspective);
    d_matRotation = glm::conjugate(d_matRotation);

    this->_quatRotation = d_matRotation;
    this->_matRotation = toMat4(this->_quatRotation);
    this->eulerAngles = vec3(glm::degrees(this->_matRotation[2][0]), glm::degrees(this->_matRotation[2][1]), glm::degrees(this->_matRotation[2][2]));
    return this->eulerAngles;
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

    vec3 up;
    // up = this->_quatRotation * vec3_up;
    up = glm::rotate(glm::inverse(this->_quatRotation), vec3_up);

    // up.x = 2 * (this->_quatRotation.x * this->_quatRotation.y + this->_quatRotation.w * this->_quatRotation.z);
    // up.y = 2 * (this->_quatRotation.x * this->_quatRotation.x + this->_quatRotation.z * this->_quatRotation.z);
    // up.z = 1 - 2 * (this->_quatRotation.y * this->_quatRotation.z + this->_quatRotation.w * this->_quatRotation.x);
    return normalize(up);
}

vec3 Transform::Down(){
    vec3 down;
    // down = this->_quatRotation * vec3_down;
    down = glm::rotate(glm::inverse(this->_quatRotation), vec3_down);

    return normalize(down);
}

vec3 Transform::Left(){

    vec3 left;
    // left = this->_quatRotation * vec3_left;
    left = glm::rotate(glm::inverse(this->_quatRotation), vec3_left);

    // left.x = 2 * (this->_quatRotation.y * this->_quatRotation.y + this->_quatRotation.z * this->_quatRotation.z);
    // left.y = 2 * (this->_quatRotation.x * this->_quatRotation.y + this->_quatRotation.w * this->_quatRotation.z);
    // left.z = 1 - 2 * (this->_quatRotation.x * this->_quatRotation.z + this->_quatRotation.w * this->_quatRotation.y);
    return normalize(left);
}

vec3 Transform::Right(){
    vec3 right;
    right = glm::rotate(glm::inverse(this->_quatRotation), vec3_right);
    // right = this->_quatRotation * vec3_right;
    return normalize(right);
}

vec3 Transform::Forward(){
    // mat4 inverted = glm::inverse(this->modelMatrix);
    // vec3 forward = glm::normalize(glm::vec3(inverted[2]));
    // return forward * vec3(1, 1, -1);

    vec3 forward;
    // forward = this->_quatRotation * vec3_forward;
    forward = glm::rotate(glm::inverse(this->_quatRotation), vec3_forward);

    // forward.x = 2 * (this->_quatRotation.x * this->_quatRotation.z + this->_quatRotation.w * this->_quatRotation.y);
    // forward.y = 2 * (this->_quatRotation.y * this->_quatRotation.z + this->_quatRotation.w * this->_quatRotation.x);
    // forward.z = 1 - 2 * (this->_quatRotation.x * this->_quatRotation.x + this->_quatRotation.y * this->_quatRotation.y);
    return normalize(forward);
}

vec3 Transform::Back(){
    vec3 back;
    back = this->_quatRotation * vec3_back;
    return normalize(back);
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

quat Transform::GetQuaternion(){
    return this->_quatRotation;
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