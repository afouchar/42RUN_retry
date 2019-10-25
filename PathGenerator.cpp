#include "PathGenerator.hpp"

PathGenerator::~PathGenerator(){}

PathGenerator::PathGenerator(){}

PathGenerator::PathGenerator(Shader *shader, unsigned int chunksAmount, float speed){

    if (chunksAmount < 3)
        chunksAmount = 3;

    this->_chunksAmount = chunksAmount;
    this->speed = speed;
    this->_shader = shader;
    this->pathAngle = 0.0f;
    this->firstTimeIn = true;

    //GET LENGTH CHUNK PROPERLY !!!
    this->_chunkLength = 40.0f;

    pathForward = Object(this->_shader, "models/tubecube/tubecube.obj");
    pathForward.SetTag("forward");
    pathTurn = Object(this->_shader, "models/tubecube/tubecube_turn.obj");
    pathTurn.SetTag("turn");

    srand(time(0));

    for (int i = 0; i < this->_chunksAmount; i++){

        if (i == 0)
            this->chunks.push_back(Object(pathForward));
        else
            this->chunks.push_back(RandomChunkFromLast());
    }
    
    //parenting
    for (list<Object>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
        if (it != this->chunks.begin()){
            list<Object>::iterator it_previous = prev(it);
            it->transform.parent = &it_previous->transform;
        }
        if (it != prev(this->chunks.end())){
            list<Object>::iterator it_next = next(it, +1);
            it->transform.child = &it_next->transform;
        }
    }

    //position relative to parent
    for (list<Object>::iterator it = next(this->chunks.begin(), 1); it != this->chunks.end(); it++){
        SetPositionFromParent((*it));
    }

    this->chunks.begin()->transform.UpdateMatrix();
}

void PathGenerator::SwapFirstToLast(){

    RenderPipeline::ClearBuffers(this->chunks.begin()->shader, this->chunks.begin()->meshes, false);
    // this->chunks.erase(this->chunks.begin());
    list<Object>::iterator it_to_delete;
    this->chunks.remove((*it_to_delete));

    this->chunks.push_back(RandomChunkFromLast());
    this->chunks.begin()->transform.LocalToWorld();

    //parenting
    this->chunks.begin()->transform.parent = nullptr;
    next(this->chunks.end(), -2)->transform.child = & next(this->chunks.end(), -1)->transform; 
    next(this->chunks.end(), -1)->transform.parent = & next(this->chunks.end(), -2)->transform; 
    next(this->chunks.end(), -1)->transform.child = nullptr;

    SetPositionFromParent((*next(this->chunks.end(), -1)));

    int i = 0;
    for (list<Object>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
        std::cout << "[" << i << "] " << it->transform.GetTag() << std::endl;
        i++;
    }
    std::cout << "==========================" << std::endl << std::endl;
}

void PathGenerator::MovePath(Object & player, float deltaTime){

    list<Object>::iterator it = this->chunks.begin();

    //OnColliderEnter
    //OnColliderStay
    //OnColliderExit
    //!!! Check rotation parfaite sinon le vaiseau parait d'ecal'e par rapport au centre !!!
    if (it->collider.CheckCollision(player.collider) && it->transform.GetTag() == "turn"){
            if (this->firstTimeIn){
                it->transform.pivot = it->transform.position + (it->transform.Right() * GetHalfChunkLength()) + (it->transform.Back() * GetHalfChunkLength());
                this->firstTimeIn = false;
            }
        it->transform.RotateAround(it->transform.pivot, it->transform.Up(), this->speed * deltaTime);
    }
    else{
        it->transform.Translate(vec3_back * this->speed * deltaTime);
        this->firstTimeIn = true;
    }
}

float PathGenerator::GetChunkLength(){
    return this->_chunkLength;
}

float PathGenerator::GetHalfChunkLength(){
    return this->_chunkLength / 2.0f;
}

void PathGenerator::SetPositionFromParent(Object &chunk){

    chunk.transform.Translate(vec3_forward * this->_chunkLength);
    if (chunk.transform.parent->GetTag() == "turn"){
        chunk.transform.RotateAround(vec3_zero, vec3_up, -90.0f);
    }
    if (chunk.transform.GetTag() == "turn"){
        chunk.transform.Rotate(vec3_forward, rand() % 360);
    }
}

Object PathGenerator::RandomChunkFromLast(){
    return RandomChunk((*next(this->chunks.end(), -1)));
}

Object PathGenerator::RandomChunk(Object previousChunk){

    Object objectChunk;

    if (previousChunk.GetTag() != "turn" && rand() % 100 < 15)
        objectChunk = Object(pathTurn);
    else
        objectChunk = Object(pathForward);

    return objectChunk;    
}