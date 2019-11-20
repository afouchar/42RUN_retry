#include "Ship.hpp"
#include "GameBehaviour.hpp"


Ship::~Ship(){}

Ship::Ship(){
}

Ship::Ship(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    this->_speed = 50.0f;
    this->healthPoint = MAX_HEALTH;
    this->ammunitions = MAX_AMMO;
    this->_bullet = new Bullet((*this->shader), "Models/Colliders/rocket.obj", false, false);
    this->_bullet->transform.SetTag("Bullet");
}

Ship::Ship(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    this->_speed = 50.0f;
    this->healthPoint = MAX_HEALTH;
    this->ammunitions = MAX_AMMO;
    this->_bullet = new Bullet((*this->shader), "Models/Colliders/rocket.obj", false, false);
    this->_bullet->transform.SetTag("Bullet");
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
    else if (collider.transform->GetTag() == "Heal"){
        this->healthPoint += 10;
        this->healthPoint = this->healthPoint > MAX_HEALTH ? MAX_HEALTH : this->healthPoint;
            std::cerr << "HEAL ! (" << this->healthPoint << "/" << MAX_HEALTH << ")" << std::endl;
    }
    else if (collider.transform->GetTag() == "Ammo"){
        this->ammunitions += 20;
        this->ammunitions = this->ammunitions > MAX_AMMO ? MAX_AMMO : this->ammunitions;
            std::cerr << "RELOAD ! (" << this->ammunitions << "/" << MAX_AMMO << ")" << std::endl;
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
            Bullet *shoot = new Bullet((*this->_bullet));
            shoot->transform.position = (this->transform.position + (vec3_forward * 2.0f));
            shoot->Launch();
        }
        else
            std::cerr << "YOU NEED TO RELOAD ! (" << this->ammunitions << "/" << MAX_AMMO << ")" << std::endl;

    }
}