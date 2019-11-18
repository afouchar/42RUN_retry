#include "Obstacle.hpp"


Obstacle::~Obstacle(){}

Obstacle::Obstacle(){}

Obstacle::Obstacle(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){}

Obstacle::Obstacle(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){}

void Obstacle::OnColliderEnter(Collider & collider){

    if (collider.transform->GetTag() == "Player")
        std::cerr << "Collide Obstacle [" << collider.transform->gameObject->ID << "]" << std::endl;
}

void Obstacle::OnColliderStay(Collider & collider){}

void Obstacle::OnColliderExit(Collider & collider){}

void Obstacle::Update(){}

void Obstacle::LateUpdate(){}