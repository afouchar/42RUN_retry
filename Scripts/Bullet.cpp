#include "Bullet.hpp"
#include "GameBehaviour.hpp"

Bullet::~Bullet(){
}

Bullet::Bullet(){
}

Bullet::Bullet(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    this->_speed = 35.0f;
    this->_launch = false;
    this->_delete = false;
}

Bullet::Bullet(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    this->_speed = 35.0f;
    this->_launch = false;
    this->_delete = false;
}

void Bullet::OnColliderEnter(Collider & collider){

    if (collider.transform->GetTag() == "Obstacle"){
        std::cerr << "bullet collision -> DESTROY !" << std::endl;
        // delete(collider.transform->gameObject);
        this->_delete = true;
    }
}

void Bullet::OnColliderStay(Collider & collider){}

void Bullet::OnColliderExit(Collider & collider){}

void Bullet::Update(){
    if (this->_launch){
        float move = this->_speed * GameBehaviour::DeltaTime();
        this->_distance += move;
        this->transform.Translate(vec3_forward * move);
        if (this->_distance >= MAX_DISTANCE){
            this->_launch = false;
            this->_delete = true;
        }
    }
}

void Bullet::LateUpdate(){

    if (this->_delete){
        delete this;
        std::cerr << "END BULLET" << std::endl;
    }
}

void Bullet::Launch(){
    this->_launch = true;
    this->_distance = 0;
}