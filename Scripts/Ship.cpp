#include "Ship.hpp"
#include "GameBehaviour.hpp"


Ship::~Ship(){}

Ship::Ship(){
}

Ship::Ship(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    this->_speed = 100.0f;
    // this->collider.position = this->transform.Up() * 20.0f;
}

Ship::Ship(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    this->_speed = 100.0f;
    // this->collider.position = this->transform.Up() * 20.0f;
}

void Ship::OnColliderEnter(Collider & collider){}

void Ship::OnColliderStay(Collider & collider){}

void Ship::OnColliderExit(Collider & collider){}

void Ship::Update(){

    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_A)){
        this->transform.RotateAround(vec3_zero, vec3_forward, this->_speed * GameBehaviour::DeltaTime());
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_D)){
        this->transform.RotateAround(vec3_zero, vec3_back, this->_speed * GameBehaviour::DeltaTime());
    }
}