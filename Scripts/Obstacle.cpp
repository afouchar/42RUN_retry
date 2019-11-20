#include "Obstacle.hpp"


Obstacle::~Obstacle(){}

Obstacle::Obstacle(){}

Obstacle::Obstacle(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
}

Obstacle::Obstacle(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
}

void Obstacle::OnColliderEnter(Collider & collider){

    // if (collider.transform->GetTag() == "Player")
    //     std::cerr << "Obstacle Collide with Player [" << collider.transform->gameObject->ID << "]" << std::endl;
    // if (collider.transform->GetTag() == "Bullet"){
    //     delete(this);
    // }
}

void Obstacle::OnColliderStay(Collider & collider){}

void Obstacle::OnColliderExit(Collider & collider){}

void Obstacle::Update(){}

void Obstacle::LateUpdate(){}