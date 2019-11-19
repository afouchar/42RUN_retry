#include "Bound.hpp"
#include <iostream>

Bound::~Bound(){}
Bound::Bound(){}
Bound::Bound(vec3 min, vec3 max, Transform & transform){

    this->transform = &transform;

    SetBound(min, max);

    points.push_back(&this->frontLeftDown);
    points.push_back(&this->frontLeftUp);
    points.push_back(&this->frontRightUp);
    points.push_back(&this->frontRightDown);
    points.push_back(&this->backLeftDown);
    points.push_back(&this->backLeftUp);
    points.push_back(&this->backRightUp);
    points.push_back(&this->backRightDown);

    this->up = vec3_up;
    this->down = vec3_down;
    this->left = vec3_left;
    this->right = vec3_right;
    this->forward = vec3_forward;
    this->back = vec3_back;

    normals.push_back(&this->up);
    normals.push_back(&this->down);
    normals.push_back(&this->left);
    normals.push_back(&this->right);
    normals.push_back(&this->forward);
    normals.push_back(&this->back);
}

Bound::Bound(const Bound & rhs){

    this->transform = rhs.transform;

    this->frontLeftDown = rhs.frontLeftDown;
    this->frontLeftUp = rhs.frontLeftUp;
    this->frontRightUp = rhs.frontRightUp;
    this->frontRightDown = rhs.frontRightDown;
    this->backLeftDown = rhs.backLeftDown;
    this->backLeftUp = rhs.backLeftUp;
    this->backRightUp = rhs.backRightUp;
    this->backRightDown = rhs.backRightDown;

    this->up = rhs.up;
    this->down = rhs.down;
    this->left = rhs.left;
    this->right = rhs.right;
    this->forward = rhs.forward;
    this->back = rhs.back;

    this->scale = rhs.scale;
    this->size = rhs.size;
    this->halfSize = rhs.halfSize;
    this->min = rhs.min;
    this->max = rhs.max;

    points.push_back(&this->frontLeftDown);
    points.push_back(&this->frontLeftUp);
    points.push_back(&this->frontRightUp);
    points.push_back(&this->frontRightDown);
    points.push_back(&this->backLeftDown);
    points.push_back(&this->backLeftUp);
    points.push_back(&this->backRightUp);
    points.push_back(&this->backRightDown);

    normals.push_back(&this->up);
    normals.push_back(&this->down);
    normals.push_back(&this->left);
    normals.push_back(&this->right);
    normals.push_back(&this->forward);
    normals.push_back(&this->back);
}

void Bound::SetBound(vec3 min, vec3 max){

    this->min = min;
    this->max = max;

    UpdateBoundValues();
}
void Bound::UpdateBoundValues(Transform & transform) {

    this->transform = &transform;
    UpdateBoundValues();
}

void Bound::UpdateBoundValues() {

    if (this->transform != nullptr)
        this->scale = this->transform->scale;
    else
        this->scale = vec3_one;

    this->size.x = glm::distance(this->max.x, this->min.x);
    this->size.y = glm::distance(this->max.y, this->min.y);
    this->size.z = glm::distance(this->max.z, this->min.z);

    this->halfSize = this->size / 2.0f;

    this->backLeftDown = this->min;
    this->backLeftUp = this->backLeftDown + (this->size * vec3_up);
    this->backRightDown = this->backLeftDown + (this->size * vec3_right);
    this->backRightUp = this->backLeftUp + (this->size * vec3_right);

    this->frontRightUp = this->max;
    this->frontRightDown = this->frontRightUp + (this->size * vec3_down);
    this->frontLeftUp = this->frontRightUp + (this->size * vec3_left);
    this->frontLeftDown = this->frontRightDown + (this->size * vec3_left);

    this->frontLeftDown *= this->scale;
    this->frontLeftUp *= this->scale;
    this->frontRightUp *= this->scale;
    this->frontRightDown *= this->scale;
    this->backLeftDown *= this->scale;
    this->backLeftUp *= this->scale;
    this->backRightUp *= this->scale;
    this->backRightDown *= this->scale;
}