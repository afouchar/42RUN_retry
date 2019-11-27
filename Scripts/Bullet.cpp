#include "Bullet.hpp"
#include "GameBehaviour.hpp"
#include "RenderPipeline.hpp"
#include "Ship.hpp"


Bullet::~Bullet(){
}

Bullet::Bullet(){
}

Bullet::Bullet(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    this->_speed = 35.0f;
    this->_delete = false;
}

Bullet::Bullet(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    this->_speed = 35.0f;
    this->_delete = false;
}

void Bullet::OnColliderEnter(Collider & collider){

    if (collider.transform->GetTag() == "Obstacle"){
        collider.isTrigger = true;
        RenderPipeline::RemoveObject((*collider.transform->gameObject), false);
        this->_delete = true;
    }
    else if (collider.transform->GetTag() == "Heal"){
        collider.isTrigger = true;
        RenderPipeline::RemoveObject((*collider.transform->gameObject), false);
        Ship::instance->Heal();
        this->_delete = true;
    }
    else if (collider.transform->GetTag() == "Ammo"){
        collider.isTrigger = true;
        RenderPipeline::RemoveObject((*collider.transform->gameObject), false);
        Ship::instance->ReloadAmmunitions();
        this->_delete = true;
    }
    else if (collider.transform->GetTag() == "Turn"){
        this->_delete = true;
    }
}

void Bullet::OnColliderStay(Collider & collider){}

void Bullet::OnColliderExit(Collider & collider){}

void Bullet::Update(){

    float move = this->_speed * GameBehaviour::DeltaTime();
    this->transform.Translate(vec3_forward * move);
    this->transform.Rotate(vec3_forward, move * 5.0f);
}

void Bullet::LateUpdate(){

    if (this->_delete){
        delete this;
    }
}