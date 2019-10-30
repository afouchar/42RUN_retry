#include "Collider.hpp"
#include "GameBehaviour.hpp"

// std::list<Collider *> Collider::_sceneColliders;

// #include <iostream>

Collider::~Collider(){
    GameBehaviour::RemoveCollider((*this));
}

Collider::Collider(const Collider & rhs){
    this->position = rhs.position;
    this->max = rhs.max;
    this->min = rhs.min;
    this->_size = rhs._size;
    this->transform = rhs.transform;
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(const Collider & rhs, Transform & transform){
    this->position = rhs.position;
    this->max = rhs.max;
    this->min = rhs.min;
    this->_size = rhs._size;
    this->transform = &transform;
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(){
    this->position = vec3_zero;
    this->min = vec3_zero;
    this->max = vec3_zero;
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(Transform & transform, vec3 minValues, vec3 maxValues){

    this->position = vec3_zero;
    this->min = minValues;
    this->max = maxValues;
    this->transform = &transform;
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(Transform & transform, vec3 minValues, vec3 maxValues, vec3 offset){

    this->position = offset;
    this->min = minValues;
    this->max = maxValues;
    this->transform = &transform;
    GameBehaviour::AddCollider((*this));
}

bool Collider::CheckCollision(Collider collider){

    UpdateCollider();
    vec3 worldPosition = GetWorldPosition() - (this->_size / 2.0f);
    vec3 colliderWorldPosition = collider.GetWorldPosition() - (collider.GetSize() / 2.0f);

    //rotate points

    bool collisionX = worldPosition.x + this->_size.x >= colliderWorldPosition.x &&
        colliderWorldPosition.x + collider.GetSize().x >= worldPosition.x;
    bool collisionY = worldPosition.y + this->_size.y >= colliderWorldPosition.y &&
        colliderWorldPosition.y + collider.GetSize().y >= worldPosition.y;
    bool collisionZ = worldPosition.z + this->_size.z >= colliderWorldPosition.z &&
        colliderWorldPosition.z + collider.GetSize().z >= worldPosition.z;

    return collisionX && collisionY && collisionZ;
}

bool Collider::CheckCollision(vec3 point){

    UpdateCollider();
    vec3 worldPosition = GetWorldPosition();
    vec3 rotMax = this->transform->GetQuaternion() * this->max;
    vec3 rotMin = this->transform->GetQuaternion() * this->min;

    bool collisionX = point.x < worldPosition.x + rotMax.x && point.x > worldPosition.x + rotMin.x;
    bool collisionY = point.y < worldPosition.y + rotMax.y && point.y > worldPosition.y + rotMin.y;
    bool collisionZ = point.z < worldPosition.z + rotMax.z && point.z > worldPosition.z + rotMin.z;

    return collisionX && collisionY && collisionZ;
}

void Collider::UpdateCollider(){

    this->_size.x = glm::distance(this->max.x, this->min.x);
    this->_size.y = glm::distance(this->max.y, this->min.y);
    this->_size.z = glm::distance(this->max.z, this->min.z);
    this->_size = this->transform->GetQuaternion() * this->_size;
}

vec3 Collider::GetSize(){

    UpdateCollider();
    return this->_size;
}

vec3 Collider::GetWorldPosition(){

    return this->position + this->transform->LocalToWorldPosition();
}