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
    this->chunksBeforeSwap = 1;

    this->_colliderCenter = new Object((*this->_shader), "Models/Colliders/small_collider.obj", true, true);
    this->_colliderCenter->SetTag("Cursor");

    this->_pathForward = new Chunk((*this), (*this->_shader), "Models/tubecube/tubecube.obj", false, false);
    this->_pathForward->SetTag("Forward");
    this->_pathTurn = new Chunk((*this), (*this->_shader), "Models/tubecube/tubecube_turn.obj", false, false);
    this->_pathTurn->SetTag("Turn");


    this->_chunkLength = this->_pathForward->collider.GetSize().z;

    srand(time(0));

    for (int i = 0; i < this->_chunksAmount; i++){

        if (i == 2)
            this->chunks.push_back(new Chunk((*this), (*this->_pathTurn)));
        else
            this->chunks.push_back(new Chunk((*this), (*this->_pathForward)));


        // if (i == 0 || i == 1)
        //     this->chunks.push_back(new Chunk((*this), (*this->_pathForward)));
        // else
        //     this->chunks.push_back(RandomChunkFromLast());
    }
    
    //parenting
    for (list<Chunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
        if (it == this->chunks.begin())
            continue;
        list<Chunk*>::iterator it_prev = prev(it, 1);
        (*it)->transform.AddParent((*it_prev)->transform);
    }

    //position relative to parent
    // (*this->chunks.begin())->transform.Translate(vec3(0, 0, this->_chunkLength + this->GetHalfChunkLength()));
    for (list<Chunk *>::iterator it = next(this->chunks.begin(), 1); it != this->chunks.end(); it++){
        SetPositionFromParent((**it));
    }

    (*this->chunks.begin())->transform.UpdateMatrix();
}

void PathGenerator::SwapFirstToLast(){

    if (this->_chunksSwapped < this->chunksBeforeSwap){
        this->_chunksSwapped++;
        return;
    }

    // for (int j = this->chunks.size(); j <= this->_chunksAmount; j++) {

        delete (*this->chunks.begin());
        this->chunks.erase(this->chunks.begin());

        this->chunks.push_back(RandomChunkFromLast());

        list<Chunk *>::iterator firstChunk = this->chunks.begin();
        list<Chunk *>::iterator endChunk = next(this->chunks.end(), -1);
        list<Chunk *>::iterator beforeEndChunk = next(this->chunks.end(), -2);
 
        // ((*beforeEndChunk))->transform.AddChild((*endChunk)->transform);
        (*endChunk)->transform.AddParent((*beforeEndChunk)->transform);
        SetPositionFromParent((**endChunk));

        int i = 0;
        for (list<Chunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
            std::cout << "[" << (*it)->ID << "] " << (*it)->transform.GetTag() << std::endl;
            i++;
        }
        std::cout << "==========================" << std::endl << std::endl;
    // }
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
    // chunk.transform.position = chunk.transform.parent->Forward() * this->_chunkLength;
    if (chunk.transform.parent->GetTag() == "Turn"){
        chunk.transform.RotateAround(vec3_zero, vec3_up, -90.0f);
    }
    if (chunk.transform.GetTag() == "Turn"){
        chunk.transform.Rotate(chunk.transform.parent->Forward(), rand() % 360);
    }
    chunk.transform.UpdateMatrix();
}

// void PathGenerator::SetPositionFromParent(Chunk & chunk){

//     if (chunk.transform.parent == nullptr)
//         return;

//     chunk.transform.position = vec3_forward * this->_chunkLength;
//     if (chunk.transform.parent->GetTag() == "Turn"){
//         chunk.transform.RotateAround(vec3_zero, vec3_up, -90.0f);
//     }
//     if (chunk.transform.GetTag() == "Turn"){
//         chunk.transform.Rotate(vec3_forward, rand() % 360);
//     }
//     chunk.transform.UpdateMatrix();
// }

Chunk *PathGenerator::RandomChunkFromLast(){

    list<Chunk *>::iterator lastChunk = next(this->chunks.end(), -1);
    return RandomChunk((**lastChunk));
}

Chunk *PathGenerator::RandomChunk(Chunk & previousChunk){

    if (previousChunk.GetTag() == "Forward" && rand() % 100 < 15)
        return new Chunk((*this), (*this->_pathTurn));
    else
        return new Chunk((*this), (*this->_pathForward));
}