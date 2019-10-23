#include "Collider.hpp"

std::list<Collider *> Collider::_sceneColliders;
#include <iostream>
Collider::~Collider(){
    Collider::_sceneColliders.remove(this);
    std::cout << "REMOVE _ size : " << Collider::_sceneColliders.size() << std::endl;
}

Collider::Collider(const Collider & rhs){
    this->position = rhs.position;
    this->max = rhs.max;
    this->min = rhs.min;
    this->_size = rhs._size;
    this->transform = rhs.transform;
    Collider::_sceneColliders.push_back(this);
}

Collider::Collider(const Collider & rhs, Transform & transform){
    this->position = rhs.position;
    this->max = rhs.max;
    this->min = rhs.min;
    this->_size = rhs._size;
    this->transform = &transform;
    Collider::_sceneColliders.push_back(this);
}

Collider::Collider(){
    this->position = vec3_zero;
    this->min = vec3_zero;
    this->max = vec3_zero;
    Collider::_sceneColliders.push_back(this);
}

Collider::Collider(Transform & transform, vec3 minValues, vec3 maxValues){

    this->position = vec3_zero;
    this->min = minValues;
    this->max = maxValues;
    this->transform = &transform;
    Collider::_sceneColliders.push_back(this);
}

Collider::Collider(Transform & transform, vec3 minValues, vec3 maxValues, vec3 offset){

    this->position = offset;
    this->min = minValues;
    this->max = maxValues;
    this->transform = &transform;
    Collider::_sceneColliders.push_back(this);
}

bool Collider::CheckCollision(Collider collider){

    UpdateCollider();
    vec3 worldPosition = GetWorldPosition() - (this->_size / 2.0f);
    vec3 colliderWorldPosition = collider.GetWorldPosition() - (collider.GetSize() / 2.0f);

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

    bool collisionX = point.x < worldPosition.x + this->max.x && point.x > worldPosition.x + this->min.x;
    bool collisionY = point.y < worldPosition.y + this->max.y && point.y > worldPosition.y + this->min.y;
    bool collisionZ = point.z < worldPosition.z + this->max.z && point.z > worldPosition.z + this->min.z;

    return collisionX && collisionY && collisionZ;
}

void Collider::UpdateCollider(){

    this->_size.x = glm::distance(this->max.x, this->min.x);
    this->_size.y = glm::distance(this->max.y, this->min.y);
    this->_size.z = glm::distance(this->max.z, this->min.z);
}

vec3 Collider::GetSize(){

    UpdateCollider();
    return this->_size;
}

vec3 Collider::GetWorldPosition(){

    return this->position + this->transform->LocalToWorldPosition();
}