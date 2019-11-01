#include "PathGenerator.hpp"
#include "RenderPipeline.hpp"

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
    this->pathAngle = 0.0f;
    this->firstTimeIn = true;


    this->_pathForward = new Chunk((*this->_shader), "Models/tubecube/tubecube.obj", false);
    this->_pathForward->SetTag("forward");
    this->_pathTurn = new Chunk((*this->_shader), "Models/tubecube/tubecube_turn.obj", false);
    this->_pathTurn->SetTag("turn");


    this->_chunkLength = this->_pathForward->collider.GetSize().z;

    srand(time(0));

    for (int i = 0; i < this->_chunksAmount; i++){

        if (i == 0)
            this->chunks.push_back(new Chunk((*this->_pathForward)));
        else
            this->chunks.push_back(RandomChunkFromLast());
    }
    
    //parenting
    for (list<Chunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
        if (it != this->chunks.begin()){
            list<Chunk *>::iterator it_previous = prev(it);
            (*it)->transform.parent = &(*it_previous)->transform;
        }
        if (it != prev(this->chunks.end())){
            list<Chunk*>::iterator it_next = next(it, +1);
            (*it)->transform.child = &(*it_next)->transform;
        }
    }

    //position relative to parent
    for (list<Chunk *>::iterator it = next(this->chunks.begin(), 1); it != this->chunks.end(); it++){
        SetPositionFromParent((**it));
    }

    (*this->chunks.begin())->transform.UpdateMatrix();
}

void PathGenerator::SwapFirstToLast(){

    list<Chunk *>::iterator it_to_delete = this->chunks.erase(this->chunks.begin());
    delete (*it_to_delete);

    this->chunks.push_back(RandomChunkFromLast());

    list<Chunk *>::iterator firstChunk = this->chunks.begin();
    list<Chunk *>::iterator endChunk = next(this->chunks.end(), -1);
    list<Chunk *>::iterator beforeEndChunk = next(this->chunks.end(), -2);

    (*firstChunk)->transform.LocalToWorld();

    //parenting
    (*firstChunk)->transform.parent = nullptr;
    (*beforeEndChunk)->transform.child = & (*endChunk)->transform; 
    (*endChunk)->transform.parent = & (*beforeEndChunk)->transform; 
    (*endChunk)->transform.child = nullptr;

    SetPositionFromParent((**endChunk));

    int i = 0;
    for (list<Chunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
        std::cout << "[" << i << "] " << (*it)->transform.GetTag() << std::endl;
        i++;
    }
    std::cout << "==========================" << std::endl << std::endl;
}

void PathGenerator::MovePath(Object & player, float deltaTime){

    list<Chunk *>::iterator it = this->chunks.begin();

    //OnColliderEnter
    //OnColliderStay
    //OnColliderExit
    //!!! Check rotation parfaite sinon le vaiseau parait d'ecal'e par rapport au centre !!!
    if ((*it)->collider.CheckCollision(player.collider) && (*it)->transform.GetTag() == "turn"){
            if (this->firstTimeIn){
                (*it)->transform.pivot = (*it)->transform.position + ((*it)->transform.Right() * GetHalfChunkLength()) + ((*it)->transform.Back() * GetHalfChunkLength());
                this->firstTimeIn = false;
            }
        (*it)->transform.RotateAround((*it)->transform.pivot, (*it)->transform.Up(), this->speed * deltaTime);
    }
    else{
        (*it)->transform.Translate(vec3_back * this->speed * deltaTime);
        this->firstTimeIn = true;
    }
}

float PathGenerator::GetChunkLength(){
    return this->_chunkLength;
}

float PathGenerator::GetHalfChunkLength(){
    return this->_chunkLength / 2.0f;
}

void PathGenerator::SetPositionFromParent(Chunk & chunk){

    chunk.transform.position = vec3_forward * this->_chunkLength;
    if (chunk.transform.parent->GetTag() == "turn"){
        chunk.transform.RotateAround(vec3_zero, vec3_up, -90.0f);
    }
    if (chunk.transform.GetTag() == "turn"){
        chunk.transform.Rotate(vec3_forward, rand() % 360);
    }
}

Chunk *PathGenerator::RandomChunkFromLast(){

    list<Chunk *>::iterator lastChunk = next(this->chunks.end(), -1);
    return RandomChunk((**lastChunk));
}

Chunk *PathGenerator::RandomChunk(Chunk & previousChunk){

    if (previousChunk.GetTag() != "turn" && rand() % 100 < 15)
        return new Chunk((*this->_pathTurn));
    else
        return new Chunk((*this->_pathForward));
}