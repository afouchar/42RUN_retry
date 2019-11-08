#include "Collider.hpp"
#include "GameBehaviour.hpp"

// std::list<Collider *> Collider::_sceneColliders;

// #include <iostream>

Collider::~Collider(){
    // std::cout << "Destroy COLLIDER (OBJ id : " << this->transform->gameObject->ID << ")" << std::endl;
    GameBehaviour::RemoveCollider((*this));
}

Collider::Collider(const Collider & rhs, bool isTrigger){
    this->position = rhs.position;
    this->bound = Bound(rhs.bound);
    this->transform = rhs.transform;
    this->isTrigger = isTrigger;
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(const Collider & rhs, Transform & transform, bool isTrigger){
    this->position = rhs.position;
    this->bound = Bound(rhs.bound);    
    this->transform = &transform;
    this->isTrigger = isTrigger;
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(bool isTrigger){
    this->position = vec3_zero;
    this->transform = nullptr;
    this->isTrigger = isTrigger;
    this->bound = Bound(vec3_zero, vec3_zero, (*this->transform));
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(Transform & transform, vec3 minValues, vec3 maxValues, bool isTrigger){

    this->position = vec3_zero;
    this->transform = &transform;
    this->isTrigger = isTrigger;
    this->bound = Bound(minValues, maxValues, (*this->transform));
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(Transform & transform, vec3 minValues, vec3 maxValues, vec3 offset, bool isTrigger){

    this->position = offset;
    this->transform = &transform;
    this->isTrigger = isTrigger;
    this->bound = Bound(minValues, maxValues, (*this->transform));
    GameBehaviour::AddCollider((*this));
}

bool Collider::CheckCollision(Collider & collider){

    Bound bound1 = Collider::BoundToWorld(this->bound);
    Bound bound2 = Collider::BoundToWorld(collider.bound);

    vec3 bound1Pos = this->GetOffsetWorldPosition();
    vec3 bound2Pos = collider.GetOffsetWorldPosition();
    float boundsMagnitude = glm::abs(glm::length(bound1.size)) + glm::abs(glm::length(bound2.size));

    if (glm::distance(bound2Pos, bound1Pos) > boundsMagnitude)
        return false;

    vec3 RPos = bound2Pos - bound1Pos;
    return GetCollision(bound1, bound2, RPos);
}

bool Collider::GetSeparatingPlane(const vec3 & RPos, const vec3 plane, const Bound & bound1, const Bound &bound2){

    return glm::abs(glm::dot(RPos, plane)) > (glm::abs(glm::dot((bound1.right * bound1.halfSize.x), plane))
                                            + glm::abs(glm::dot((bound1.up * bound1.halfSize.y), plane))
                                            + glm::abs(glm::dot((bound1.back * bound1.halfSize.z), plane))
                                            + glm::abs(glm::dot((bound2.right * bound2.halfSize.x), plane))
                                            + glm::abs(glm::dot((bound2.up * bound2.halfSize.y), plane))
                                            + glm::abs(glm::dot((bound2.back * bound2.halfSize.z), plane)));
}

bool Collider::GetCollision(const Bound & bound1, const Bound & bound2, const vec3 & RPos){

    if (GetSeparatingPlane(RPos, bound1.right, bound1, bound2)
        || GetSeparatingPlane(RPos, bound1.up, bound1, bound2)
        || GetSeparatingPlane(RPos, bound1.back, bound1, bound2)
        || GetSeparatingPlane(RPos, bound2.right, bound1, bound2)
        || GetSeparatingPlane(RPos, bound2.up, bound1, bound2)
        || GetSeparatingPlane(RPos, bound2.back, bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.right, bound2.right), bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.right, bound2.up), bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.right, bound2.back), bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.up, bound2.right), bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.up, bound2.up), bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.up, bound2.back), bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.back, bound2.right), bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.back, bound2.up), bound1, bound2)
        || GetSeparatingPlane(RPos, glm::cross(bound1.back, bound2.back), bound1, bound2))
        return false;

    return true;
}


bool Collider::CheckCollision(vec3 point){

    //change to new collision/intersection detection ?!

    UpdateCollider();
    vec3 worldPosition = GetOffsetWorldPosition();
    vec3 rotMax = this->transform->GetQuaternion() * this->bound.max;
    vec3 rotMin = this->transform->GetQuaternion() * this->bound.min;

    bool collisionX = point.x < worldPosition.x + rotMax.x && point.x > worldPosition.x + rotMin.x;
    bool collisionY = point.y < worldPosition.y + rotMax.y && point.y > worldPosition.y + rotMin.y;
    bool collisionZ = point.z < worldPosition.z + rotMax.z && point.z > worldPosition.z + rotMin.z;

    return collisionX && collisionY && collisionZ;
}

void Collider::UpdateCollider(){

    this->bound.size.x = glm::distance(this->bound.max.x, this->bound.min.x);
    this->bound.size.y = glm::distance(this->bound.max.y, this->bound.min.y);
    this->bound.size.z = glm::distance(this->bound.max.z, this->bound.min.z);
    this->bound.size = this->transform->GetQuaternion() * this->bound.size;
    this->bound.halfSize = this->bound.size / 2.0f;
}

vec3 Collider::GetSize(){

    UpdateCollider();
    return this->bound.size;
}

Bound Collider::BoundToWorld(Bound & bound){
    
    if (bound.transform == nullptr)
        return bound;

    Bound tempBound = Bound(bound);
    Collider *colPtr = &bound.transform->gameObject->collider;

    // vec3 worldPosition = colPtr->GetOffsetWorldPosition();

    // tempBound.frontLeftDown = worldPosition + colPtr->GetOffsetLocalPosition(tempBound.frontLeftDown);
    // tempBound.frontLeftUp = worldPosition + colPtr->GetOffsetLocalPosition(tempBound.frontLeftUp);
    // tempBound.frontRightUp = worldPosition + colPtr->GetOffsetLocalPosition(tempBound.frontRightUp);
    // tempBound.frontRightDown = worldPosition + colPtr->GetOffsetLocalPosition(tempBound.frontRightDown);
    // tempBound.backLeftDown = worldPosition + colPtr->GetOffsetLocalPosition(tempBound.backLeftDown);
    // tempBound.backLeftUp = worldPosition + colPtr->GetOffsetLocalPosition(tempBound.backLeftUp);
    // tempBound.backRightUp = worldPosition + colPtr->GetOffsetLocalPosition(tempBound.backRightUp);
    // tempBound.backRightDown = worldPosition + colPtr->GetOffsetLocalPosition(tempBound.backRightDown);

    tempBound.frontLeftDown = colPtr->GetOffsetLocalPosition(tempBound.frontLeftDown);
    tempBound.frontLeftUp = colPtr->GetOffsetLocalPosition(tempBound.frontLeftUp);
    tempBound.frontRightUp = colPtr->GetOffsetLocalPosition(tempBound.frontRightUp);
    tempBound.frontRightDown = colPtr->GetOffsetLocalPosition(tempBound.frontRightDown);
    tempBound.backLeftDown = colPtr->GetOffsetLocalPosition(tempBound.backLeftDown);
    tempBound.backLeftUp = colPtr->GetOffsetLocalPosition(tempBound.backLeftUp);
    tempBound.backRightUp = colPtr->GetOffsetLocalPosition(tempBound.backRightUp);
    tempBound.backRightDown = colPtr->GetOffsetLocalPosition(tempBound.backRightDown);

    tempBound.up = bound.transform->Up();
    tempBound.down = bound.transform->Down();
    tempBound.left = bound.transform->Left();
    tempBound.right = bound.transform->Right();
    tempBound.forward = bound.transform->Forward();
    tempBound.back = bound.transform->Back();

    return tempBound;
}

Bound Collider::BoundToWorld(){

    return Collider::BoundToWorld(this->bound);
}

vec3 Collider::GetOffsetLocalPosition(vec3 point){
    return this->transform->GetQuaternion() * point;
}

vec3 Collider::GetOffsetWorldPosition(){
    return GetOffsetLocalPosition(this->position) + this->transform->WorldPosition();
}