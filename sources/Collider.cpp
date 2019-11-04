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
    this->bound = Bound(vec3_zero, vec3_zero);
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(Transform & transform, vec3 minValues, vec3 maxValues, bool isTrigger){

    this->position = vec3_zero;
    this->transform = &transform;
    this->isTrigger = isTrigger;
    this->bound = Bound(minValues, maxValues);
    GameBehaviour::AddCollider((*this));
}

Collider::Collider(Transform & transform, vec3 minValues, vec3 maxValues, vec3 offset, bool isTrigger){

    this->position = offset;
    this->transform = &transform;
    this->isTrigger = isTrigger;
    this->bound = Bound(minValues, maxValues);
    GameBehaviour::AddCollider((*this));
}

bool Collider::CheckCollision(Collider & collider){

    Bound bound1 = Collider::BoundToWorld(this->bound, (*this->transform));
    Bound bound2 = Collider::BoundToWorld(collider.bound, (*collider.transform));

    return Intersects(bound1, bound2);

    // UpdateCollider();
    // vec3 worldPosition = GetOffsetWorldPosition() - (this->bound.size / 2.0f);
    // vec3 colliderWorldPosition = collider.GetOffsetWorldPosition() - (collider.GetSize() / 2.0f);

    // //rotate points

    // bool collisionX = worldPosition.x + this->bound.size.x >= colliderWorldPosition.x &&
    //     colliderWorldPosition.x + collider.GetSize().x >= worldPosition.x;
    // bool collisionY = worldPosition.y + this->bound.size.y >= colliderWorldPosition.y &&
    //     colliderWorldPosition.y + collider.GetSize().y >= worldPosition.y;
    // bool collisionZ = worldPosition.z + this->bound.size.z >= colliderWorldPosition.z &&
    //     colliderWorldPosition.z + collider.GetSize().z >= worldPosition.z;

    // return collisionX && collisionY && collisionZ;
}

bool Collider::CheckCollision(vec3 point){

    //change to new collision/intersection detection

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
}

bool Collider::Intersects(Bound bound1, Bound bound2) {

    if (!OverlapTest(bound1, bound2))
        return false;
    if (!OverlapTest(bound2, bound1))
        return false;
  return true;
}

bool Collider::OverlapTest(Bound bound1, Bound bound2){

    for(int i = 0 ; i < bound1.normals.size() ; i++) {

    float shape1Min;
    float shape1Max;
    float shape2Min;
    float shape2Max;

    SATTest((*bound1.normals[i]), bound1.points, shape1Min, shape1Max);
    SATTest((*bound1.normals[i]), bound2.points, shape2Min, shape2Max);
    if(!Overlaps(shape1Min, shape1Max, shape2Min, shape2Max)) {
      return false ; // NO INTERSECTION
    }
  }
  return true;
}

void Collider::SATTest(const vec3 & axis, const vector<vec3 *> & pointsSet, float & minAlong, float & maxAlong) {

  minAlong = HUGE;
  maxAlong = -HUGE;

  for( int i = 0 ; i < pointsSet.size() ; i++ ) {
    float dotVal = glm::dot((*pointsSet[i]), axis);
    if(dotVal < minAlong)
        minAlong = dotVal;
    if(dotVal > maxAlong)
        maxAlong = dotVal;
  }
}

bool Collider::Overlaps(float min1, float max1, float min2, float max2) {
  return IsBetweenOrdered(min2, min1, max1) || IsBetweenOrdered(min1, min2, max2) ;
}

bool Collider::IsBetweenOrdered(float val, float lowerBound, float upperBound) {
  return lowerBound <= val && val <= upperBound;
}

vec3 Collider::GetSize(){

    UpdateCollider();
    return this->bound.size;
}

Bound Collider::BoundToWorld(Bound & bound, Transform & transform){
    
    Bound tempBound = Bound(bound);
    Collider *colPtr = &transform.gameObject->collider;

    tempBound.frontLeftDown = colPtr->GetOffsetWorldPosition() + colPtr->GetOffsetLocalPosition(bound.frontLeftDown);
    tempBound.frontLeftUp = colPtr->GetOffsetWorldPosition() + colPtr->GetOffsetLocalPosition(bound.frontLeftUp);
    tempBound.frontRightUp = colPtr->GetOffsetWorldPosition() + colPtr->GetOffsetLocalPosition(bound.frontRightUp);
    tempBound.frontRightDown = colPtr->GetOffsetWorldPosition() + colPtr->GetOffsetLocalPosition(bound.frontRightDown);
    tempBound.backLeftDown = colPtr->GetOffsetWorldPosition() + colPtr->GetOffsetLocalPosition(bound.backLeftDown);
    tempBound.backLeftUp = colPtr->GetOffsetWorldPosition() + colPtr->GetOffsetLocalPosition(bound.backLeftUp);
    tempBound.backRightUp = colPtr->GetOffsetWorldPosition() + colPtr->GetOffsetLocalPosition(bound.backRightUp);
    tempBound.backRightDown = colPtr->GetOffsetWorldPosition() + colPtr->GetOffsetLocalPosition(bound.backRightDown);

    tempBound.up = transform.Up();
    tempBound.down = transform.Down();
    tempBound.left = transform.Left();
    tempBound.right = transform.Right();
    tempBound.forward = transform.Forward();
    tempBound.back = transform.Back();

    return tempBound;
}

Bound Collider::BoundToWorld(){

    return Collider::BoundToWorld(this->bound, (*this->transform));
}

vec3 Collider::GetOffsetLocalPosition(vec3 point){
    return this->transform->GetQuaternion() * point;
}

vec3 Collider::GetOffsetWorldPosition(){
    return GetOffsetLocalPosition(this->position) + this->transform->WorldPosition();
}