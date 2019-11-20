#include "PathGenerator.hpp"
#include "RenderPipeline.hpp"
#include "GameBehaviour.hpp"

// float PathGenerator::speed = 0.0f;

PathGenerator::~PathGenerator(){
    delete _pathForward;
    delete _pathTurn;
}

PathGenerator::PathGenerator(){}

PathGenerator::PathGenerator(Shader & shader, unsigned int chunksAmount, float speed){

    if (chunksAmount < 3)
        chunksAmount = 3;

    this->_chunksAmount = chunksAmount;
    this->speed = speed;
    this->_shader = &shader;
    this->_chunksSwapped = 0;
    this->chunksBeforeSwap = 2;
    this->_lastTurnAngle = 0.0f;

    this->_colliderCenter = new Object((*this->_shader), "Models/Colliders/small_collider.obj", false, true);
    this->_colliderCenter->SetTag("Cursor");

    this->_pathForward = new Chunk((*this), (*this->_shader), "Models/tubecube/tubecube.obj", false, false);
    this->_pathForward->SetTag("Forward");
    this->_pathTurn = new Chunk((*this), (*this->_shader), "Models/tubecube/tubecube_turn.obj", false, false);
    this->_pathTurn->SetTag("Turn");

    this->_obstacleType = new Obstacle((*this->_shader), "Models/Colliders/crate_obstacle.obj", false, false);
    this->_obstacleType->SetTag("Obstacle");
    this->_healType = new Obstacle((*this->_shader), "Models/Colliders/crate_heal.obj", false, false);
    this->_healType->SetTag("Heal");
    this->_ammoType = new Obstacle((*this->_shader), "Models/Colliders/crate_ammo.obj", false, false);
    this->_ammoType->SetTag("Ammo");

    this->_chunkLength = this->_pathForward->collider.GetSize().z;

    srand(time(0));

    for (int i = 0; i < this->_chunksAmount; i++){

        if (i < 2){
            Chunk *pf = new Chunk((*this), (*this->_pathForward));
            pf->SetObstacleType((*this->_obstacleType), (*this->_healType), (*this->_ammoType));
            this->chunks.push_back(pf);
        }
        else
            this->chunks.push_back(RandomChunkFromLast());
    }

    //parenting
    for (list<Chunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
        if (it == this->chunks.begin())
            continue;
        list<Chunk*>::iterator it_prev = prev(it, 1);
        (*it)->transform.AddParent((*it_prev)->transform);
    }

    //position relative to parent
    for (list<Chunk *>::iterator it = next(this->chunks.begin(), 1); it != this->chunks.end(); it++){
        SetPositionFromParent((**it));
    }

    (*this->chunks.begin())->transform.UpdateMatrix();
}

void PathGenerator::Update(){

    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_UP)){
        this->speed += 10.0f * GameBehaviour::DeltaTime();
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_DOWN)){
        this->speed -= 10.0f * GameBehaviour::DeltaTime();
    }
}

void PathGenerator::SwapFirstToLast(){

    if (this->_chunksSwapped < this->chunksBeforeSwap){
        this->_chunksSwapped++;
        return;
    }

    (*this->chunks.begin())->ClearObstacles();
    delete (*this->chunks.begin());
    this->chunks.erase(this->chunks.begin());

    // for (int i = this->chunks.size(); i <= this->_chunksAmount; i++){
        this->chunks.push_back(RandomChunkFromLast());

        list<Chunk *>::iterator firstChunk = this->chunks.begin();
        list<Chunk *>::iterator endChunk = next(this->chunks.end(), -1);
        list<Chunk *>::iterator beforeEndChunk = next(this->chunks.end(), -2);

        (*endChunk)->transform.AddParent((*beforeEndChunk)->transform, true);
        SetPositionFromParent((**endChunk));
    // }

    // int i = 0;
    // for (list<Chunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
    //     if ((*it)->transform.parent != nullptr)
    //         std::cout << "[" << (*it)->ID << "] " << (*it)->transform.GetTag() << " -> parent ["<< (*it)->transform.parent->gameObject->ID << "]" << std::endl;
    //     else
    //         std::cout << "[" << (*it)->ID << "] " << (*it)->transform.GetTag() << " -> parent [NULL]" << std::endl;
    //     i++;
    // }
    // std::cout << "==========================" << std::endl << std::endl;
}

float PathGenerator::GetChunkLength(){
    return this->_chunkLength;
}

float PathGenerator::GetHalfChunkLength(){
    return this->_chunkLength / 2.0f;
}

void PathGenerator::SetPositionFromParent(Chunk & chunk){

    if (chunk.transform.parent == nullptr)
        return;

    chunk.transform.position = vec3_forward * this->_chunkLength;

    if (chunk.transform.parent->GetTag() == "Turn"){
        chunk.transform.RotateAround(vec3_zero, vec3_up, -90.0f);
    }
    if (chunk.transform.GetTag() == "Turn"){
        this->_lastTurnAngle = GetAngleOutOfRange(this->_lastTurnAngle, 75.0f);
        chunk.transform.Rotate(vec3_forward, this->_lastTurnAngle);
    }
    if (chunk.transform.GetTag() == "Forward" && chunk.transform.parent->GetTag() == "Forward"){
        chunk.GenerateObstacles(1, 3);
    }
    chunk.transform.UpdateMatrix();
}

Chunk *PathGenerator::RandomChunkFromLast(){

    list<Chunk *>::iterator lastChunk = next(this->chunks.end(), -1);
    return RandomChunk((**lastChunk));
}

Chunk *PathGenerator::RandomChunk(Chunk & previousChunk){

    if (previousChunk.GetTag() == "Forward" && rand() % 100 < 15){
        Chunk *pt = new Chunk((*this), (*this->_pathTurn));
        pt->SetObstacleType((*this->_obstacleType), (*this->_healType), (*this->_ammoType));
        return pt;
    }
    else {
        Chunk *pf = new Chunk((*this), (*this->_pathForward));
        pf->SetObstacleType((*this->_obstacleType), (*this->_healType), (*this->_ammoType));
        return pf;
    }
}

float PathGenerator::GetAngleOutOfRange(float baseAngle, float range){

    float angle = rand() % 360;
    float maxAngle = (int)(baseAngle + range) % 360;
    float minAngle = (int)(baseAngle - range);

    minAngle = minAngle < 0 ? (360.0f + minAngle) : minAngle;

    if (angle < maxAngle && angle > minAngle)
        angle = (int)(baseAngle + range) % 360;

    return angle;
}
