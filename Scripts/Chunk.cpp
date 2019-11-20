#include "Chunk.hpp"
#include "PathGenerator.hpp"
#include "GameBehaviour.hpp"

bool Chunk::_allowSwap = true;
bool Chunk::isMoving = true;
bool Chunk::isTurning = false;
vec3 Chunk::_pivot = vec3_zero;
vec3 Chunk::_upAxis = vec3_up;
vec3 Chunk::_leftAxis = vec3_left;
vec3 Chunk::_endPosition = vec3_zero;
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

void Chunk::SetObstacleType(Obstacle & obstacle, Obstacle & heal, Obstacle & ammo){

    this->_obstacleType = &obstacle;
    // this->_obstacleType->SetTag("Obstacle");
    this->_healType = &heal;
    // this->_healType->SetTag("Heal");
    this->_ammoType = &ammo;
    // this->_ammoType->SetTag("Ammo");
}


void Chunk::OnColliderEnter(Collider & collider){

    if (collider.transform->GetTag() == "Cursor"){
        if (this->transform.parent != nullptr && this->transform.parent->GetTag() != "Turn")
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

    if (collider.transform->GetTag() == "Cursor")
        Chunk::_allowSwap = true;
}

void Chunk::Update(){

    // if (Chunk::isMoving && Chunk::_allowSwap){
    if (Chunk::_allowSwap){
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
    Chunk::_leftAxis = this->transform.Left();
    Chunk::_endPosition = this->transform.WorldPosition() + (this->transform.Back() * this->transform.gameObject->collider.bound.size.z);
    Chunk::_totalRotation = 0.0f;
}

void Chunk::Move(){
    this->transform.Translate(vec3_back * this->_generator->speed * GameBehaviour::DeltaTime());
}


void Chunk::Turn(){

    float angleRotation = this->_generator->speed * GameBehaviour::DeltaTime() * 2;
    Chunk::_totalRotation += angleRotation;

    if (Chunk::_totalRotation >= 90.0f){

        angleRotation -= (Chunk::_totalRotation - 90.0f);

        this->transform.position = Chunk::_endPosition;
        this->transform.LookAtTarget(Chunk::_endPosition + (Chunk::_leftAxis * 100.0f), Chunk::_upAxis);
        this->transform.UpdateMatrix();

        if (this->transform.child.size() > 0)
            (*this->transform.child.begin())->SetParentAsChild();

        Chunk::isMoving = true;
        Chunk::isTurning = false;
        this->isSelfTurning = false;
    }
    else{
        this->transform.RotateAround(Chunk::_pivot, Chunk::_upAxis, angleRotation);
    }
}

void Chunk::GenerateObstacles(unsigned int min, unsigned int max){

    int obstaclesCount = Transform::RandomBetween((int)min, (int)max);

    for (int i = 0; i < obstaclesCount; i++) {
        
        Obstacle *obs = RandomObstacle(15, 15);
        obs->transform.AddParent(this->transform, false);

        float halfBoundSize = this->transform.gameObject->collider.bound.halfSize.z;
        float zPos = Transform::RandomBetween(-halfBoundSize, halfBoundSize);
        float yPos = halfBoundSize - 1.0f;
        if (obs->GetTag() == "Obstacle"){
            obs->transform.position = vec3(0, yPos, zPos);
            obs->transform.RotateAround(vec3_zero, vec3_forward, rand() % 360);
            vec3 randSize = vec3_one;
            randSize.x = 18.0f;
            randSize.y = Transform::RandomBetween(5.0f, 11.0f);
            randSize.z = Transform::RandomBetween(5.0f, 8.0f);
            obs->transform.SetScale(randSize);
            obs->collider.bound.UpdateBoundValues(obs->transform);
        }
        else{
            obs->transform.position = vec3(0, yPos - 1.0f, zPos);
            obs->transform.RotateAround(vec3_zero, vec3_forward, rand() % 360);
            obs->transform.SetScale(vec3_one * 2.0f);
            obs->collider.bound.UpdateBoundValues(obs->transform);
        }
        this->obstacles.push_back(obs);
    }
}

Obstacle *Chunk::RandomObstacle(unsigned int healChance, unsigned int ammoChance){

    int val = rand() % 100;

    if (val <= healChance)
        return new Obstacle((*this->_healType));
    else if (val <= (healChance + ammoChance))
        return new Obstacle((*this->_ammoType));

    return new Obstacle((*this->_obstacleType));
}


void Chunk::ClearObstacles(){

    for (list<Obstacle *>::iterator it = this->obstacles.begin(); it != this->obstacles.end(); it++){
        delete (*it);
    }
    this->obstacles.clear();
}
