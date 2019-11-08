#include "Chunk.hpp"
#include "PathGenerator.hpp"
#include "GameBehaviour.hpp"

bool Chunk::isMoving = true;

Chunk::~Chunk(){}

Chunk::Chunk(){}

Chunk::Chunk(PathGenerator & generator, const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    this->_generator = &generator;
    this->isTurning = false;
}

Chunk::Chunk(PathGenerator & generator, Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    this->_generator = &generator;
    this->isTurning = false;
}


void Chunk::OnColliderEnter(Collider & collider){

    if (this->GetTag() == "Turn" && collider.transform->GetTag() == "Player"){
        // std::cout << "Collision Enter : [" << this->GetTag() << " : " << this->ID << "] -> [" << collider.transform->gameObject->GetTag() << " : " << collider.transform->gameObject->ID << "]" << std::endl;
        SetPivot();
        this->isTurning = true;
        Chunk::isMoving = false;
    }
    else if (this->GetTag() == "Forward" && collider.transform->GetTag() == "Player"){
        //place chunk properly
        this->isTurning = false;
        Chunk::isMoving = true;
    }
}

void Chunk::OnColliderStay(Collider & collider){

    // if (this->GetTag() == "Turn" && collider.transform->GetTag() == "Player"){
    //     this->transform.RotateAround(this->transform.pivot, this->transform.Up(), this->_generator->speed * GameBehaviour::DeltaTime());
    // }
    // else if (this->GetTag() == "Forward" && collider.transform->GetTag() == "Player"){
    //     //this->transform.Translate(vec3_back * PathGenerator::speed * GameBehaviour::DeltaTime());
    //     this->transform.GetRoot()->Translate(vec3_back * this->_generator->speed * GameBehaviour::DeltaTime());
    // }
}

void Chunk::OnColliderExit(Collider & collider){

    if (collider.transform->GetTag() == "Player"){
        if (this->GetTag() == "Turn"){
            this->isTurning = false;
        }
        std::cerr << "Swap " << this->transform.GetTag() << std::endl;
        this->_generator->SwapFirstToLast();
    }
}

void Chunk::Update(){

    if (isTurning)
        Turn();
    else if (Chunk::isMoving){
        if ((*this->transform.GetRoot()) == this->transform)
            Move();
    }
}

void Chunk::SetPivot(){
    float halfChunkLength = this->collider.bound.halfSize.z;
    this->transform.pivot = this->transform.position + (this->transform.Right() * halfChunkLength) + (this->transform.Back() * halfChunkLength);
}

void Chunk::Move(){
    this->transform.Translate(vec3_back * this->_generator->speed * GameBehaviour::DeltaTime());
}

void Chunk::Turn(){
    this->transform.RotateAround(this->transform.pivot, this->transform.Up(), this->_generator->speed * GameBehaviour::DeltaTime());
}
