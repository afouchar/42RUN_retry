#include "Chunk.hpp"
#include "PathGenerator.hpp"
#include "GameBehaviour.hpp"

bool Chunk::_allowSwap = true;
bool Chunk::isMoving = true;
bool Chunk::isTurning = false;
vec3 Chunk::_pivot = vec3_zero;
vec3 Chunk::_upAxis = vec3_up;
float Chunk::_totalRotation = 0.0f;


Chunk::~Chunk(){}

Chunk::Chunk(){}

Chunk::Chunk(PathGenerator & generator, const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    this->_generator = &generator;
    this->isSelfTurning = false;
}

Chunk::Chunk(PathGenerator & generator, Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    this->_generator = &generator;
    this->isSelfTurning = false;
    this->cleanPosition = vec3_zero;
}

void Chunk::OnColliderEnter(Collider & collider){

    if (collider.transform->GetTag() == "Cursor"){
        this->transform.SetParentAsChild();
        if (this->GetTag() == "Turn"){
            SetPivot();
            this->isSelfTurning = true;
            Chunk::isMoving = false;
            Chunk::isTurning = true;
        }
    }
}

void Chunk::OnColliderStay(Collider & collider){
}

void Chunk::OnColliderExit(Collider & collider){

    if (collider.transform->GetTag() == "Cursor"){
        Chunk::_allowSwap = true;
        if (this->GetTag() == "Turn"){

            // vec3 right = this->transform.Right();
            // std::cerr << right.x << " | " << right.y << " | " << right.z << std::endl;
            // this->transform.SetRotation(quat());
            // this->transform.LookAtTarget(vec3_right, this->transform.Up());
            // this->transform.UpdateMatrix();
        }
    }
}

void Chunk::Update(){

    if (Chunk::isMoving && Chunk::_allowSwap){
        this->_generator->SwapFirstToLast();
        Chunk::_allowSwap = false;
    }

    if (this->isSelfTurning && Chunk::isTurning){
        Turn();
    }

    if ((*this->transform.GetRoot()) == this->transform){
        if (Chunk::isMoving){
            Move();
        }
    }

}

void Chunk::SetPivot(){
    float halfChunkLength = this->collider.bound.halfSize.z;
    Chunk::_pivot = this->transform.WorldPosition()  + (this->transform.Right() * halfChunkLength) + (this->transform.Back() * halfChunkLength);
    Chunk::_upAxis = this->transform.Up();
    Chunk::_totalRotation = 0.0f;

}

void Chunk::Move(){
    this->transform.Translate(vec3_back * this->_generator->speed * GameBehaviour::DeltaTime());
    // this->transform.position = vec3(0, 0, this->transform.position.z);
}

void Chunk::Turn(){
    float angleRotation = this->_generator->speed * GameBehaviour::DeltaTime() * 2;
    Chunk::_totalRotation += angleRotation;

    // std::cerr << "TOTAL ROTATION : " << Chunk::_totalRotation << " ___ Added angle : " << angleRotation << std::endl;

    if (Chunk::_totalRotation >= 90.0f){

        // float diff = (Chunk::_totalRotation - 90.0f);
        // angleRotation -= diff;
        // Chunk::_totalRotation -= diff;
        angleRotation -= (Chunk::_totalRotation - 90.0f);
        Chunk::isMoving = true;
        Chunk::isTurning = false;
        this->isSelfTurning = false;

        // std::cerr << "TOTAL ROTATION DONE : " << Chunk::_totalRotation << " ___ Added angle : " << angleRotation << std::endl;        
    }
    this->transform.RotateAround(Chunk::_pivot, Chunk::_upAxis, angleRotation);
}
