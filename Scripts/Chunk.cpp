#include "Chunk.hpp"
#include "PathGenerator.hpp"
#include "GameBehaviour.hpp"

bool Chunk::isMoving = true;
bool Chunk::allowSwap = false;


Chunk::~Chunk(){}

Chunk::Chunk(){}

Chunk::Chunk(PathGenerator & generator, const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    this->_generator = &generator;
    this->isTurning = false;
    this->_totalRotation = 0.0f;
    this->_pivot = vec3_zero;
}

Chunk::Chunk(PathGenerator & generator, Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    this->_generator = &generator;
    this->isTurning = false;
    this->_totalRotation = 0.0f;
    this->_pivot = vec3_zero;
}


void Chunk::OnColliderEnter(Collider & collider){

    if (this->GetTag() == "Turn" && collider.transform->GetTag() == "Player"){
        if (this->transform.parent != nullptr){
            this->transform.parent->ClearParenting(this->transform);
        }
        SetPivot();
        this->_totalRotation = 0.0f;
        this->isTurning = true;
        Chunk::isMoving = false;
        Chunk::allowSwap = true;

    }
    else if (this->GetTag() == "Forward" && collider.transform->GetTag() == "Player"){
        if (this->transform.parent != nullptr){
            this->transform.parent->ClearParenting(this->transform);
        }
        this->isTurning = false;
        Chunk::isMoving = true;
        Chunk::allowSwap = true;
    }
}

void Chunk::OnColliderStay(Collider & collider){
}

void Chunk::OnColliderExit(Collider & collider){

    if (collider.transform->GetTag() == "Player"){
        if (this->GetTag() == "Turn"){
            this->isTurning = false;
            // std::cerr << "Swap [" << this->ID << "] " << this->transform.GetTag() << std::endl;
        }
        if (Chunk::allowSwap){
            Chunk::allowSwap = false;
            this->_generator->SwapFirstToLast();
        }
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
    this->_pivot = this->transform.position + (this->transform.Right() * halfChunkLength) + (this->transform.Back() * halfChunkLength);
}

void Chunk::Move(){
    this->transform.Translate(vec3_back * this->_generator->speed * GameBehaviour::DeltaTime());
}

void Chunk::Turn(){
    float angleRotation = this->_generator->speed * GameBehaviour::DeltaTime();
    this->_totalRotation += angleRotation;
    if (this->_totalRotation > 90.0f){

        angleRotation -= (this->_totalRotation - 90.0f);
        this->isTurning = false;
        Chunk::isMoving = true;
    }
    this->transform.RotateAround(this->_pivot, this->transform.Up(), angleRotation);
}
