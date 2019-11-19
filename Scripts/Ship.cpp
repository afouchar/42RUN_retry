#include "Ship.hpp"
#include "GameBehaviour.hpp"


Ship::~Ship(){}

Ship::Ship(){
}

Ship::Ship(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    this->_speed = 50.0f;
    this->healthPoint = MAX_HEALTH;
    this->ammunitions = MAX_AMMO;
    // this->collider.position = this->transform.Up() * 20.0f;
}

Ship::Ship(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    this->_speed = 50.0f;
    this->healthPoint = MAX_HEALTH;
    this->ammunitions = MAX_AMMO;
    // this->collider.position = this->transform.Up() * 20.0f;
}

void Ship::OnColliderEnter(Collider & collider){

    if (collider.transform->GetTag() == "Obstacle"){
        std::cerr << "Player Collide with Obstacle [" << collider.transform->gameObject->ID << "] __ scale : " << collider.bound.scale.x << " | " << collider.bound.scale.y << " | " << collider.bound.scale.z << std::endl;
        if (this->healthPoint > 0){
            this->healthPoint--;
            std::cerr << "PAF ! (" << this->healthPoint << "/" << MAX_HEALTH << ")" << std::endl;
        }
        else
            std::cerr << "YOU ARE DEAD (" << this->healthPoint << "/" << MAX_HEALTH << ")" << std::endl;
    }
}

void Ship::OnColliderStay(Collider & collider){}

void Ship::OnColliderExit(Collider & collider){}

void Ship::Update(){

    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_A)){
        this->transform.RotateAround(vec3_zero, vec3_forward, this->_speed * GameBehaviour::DeltaTime());
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_D)){
        this->transform.RotateAround(vec3_zero, vec3_back, this->_speed * GameBehaviour::DeltaTime());
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_SPACE)){
        if (this->ammunitions > 0){
            this->ammunitions--;
            std::cerr << "PIOU ! (" << this->ammunitions << "/" << MAX_AMMO << ")" << std::endl;
        }
        else
            std::cerr << "YOU NEED TO RELOAD ! (" << this->ammunitions << "/" << MAX_AMMO << ")" << std::endl;

    }
}