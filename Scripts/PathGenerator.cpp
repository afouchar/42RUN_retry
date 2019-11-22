#include "PathGenerator.hpp"
#include "RenderPipeline.hpp"
#include "GameBehaviour.hpp"


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
    this->_oldSpeed = this->speed;
    this->_pauseMove = false;
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
        AddMoveSpeed(10.0f * GameBehaviour::DeltaTime());
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_DOWN)){
        AddMoveSpeed(-10.0f * GameBehaviour::DeltaTime());
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_P)){
        StopMove();
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_R)){
        ResumeMove();
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_O)){
        PauseMove();
    }
}

void PathGenerator::PauseMove(){

    if (!this->_pauseMove)
        StopMove();
    else if (this->_pauseMove)
        ResumeMove();

}

void PathGenerator::StopMove(){
    if (!this->_pauseMove){
        this->_pauseMove = true;
        this->_oldSpeed = this->speed;
        this->speed = 0;
    }
}

void PathGenerator::ResumeMove(){
    if (this->_pauseMove){
        this->_pauseMove = false;
        this->speed = this->_oldSpeed;
    }
}

void PathGenerator::AddMoveSpeed(float addSpeed){
    this->speed += addSpeed;
    this->speed = this->speed > MAX_MOVE_SPEED ? MAX_MOVE_SPEED : this->speed;
    std::cerr << "Speed _________________________ : " << this->speed << std::endl;
}

void PathGenerator::SwapFirstToLast(){

    if (this->_chunksSwapped < this->chunksBeforeSwap){
        this->_chunksSwapped++;
        return;
    }

    (*this->chunks.begin())->ClearObstacles();
    delete (*this->chunks.begin());
    this->chunks.erase(this->chunks.begin());

    this->chunks.push_back(RandomChunkFromLast());

    list<Chunk *>::iterator firstChunk = this->chunks.begin();
    list<Chunk *>::iterator endChunk = next(this->chunks.end(), -1);
    list<Chunk *>::iterator beforeEndChunk = next(this->chunks.end(), -2);

    (*endChunk)->transform.AddParent((*beforeEndChunk)->transform, true);
    SetPositionFromParent((**endChunk));

    if (this->speed < 40.0f)
        AddMoveSpeed(2.0f);
    else if (this->speed < 70.0f)
        AddMoveSpeed(1.0f);
    else
        AddMoveSpeed(0.5f);
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
