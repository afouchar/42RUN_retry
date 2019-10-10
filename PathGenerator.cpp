#include "PathGenerator.hpp"

PathGenerator::~PathGenerator(){}

PathGenerator::PathGenerator(){}

PathGenerator::PathGenerator(Shader *shader, unsigned int chunksAmount, float speed){

    if (chunksAmount < 3)
        chunksAmount = 3;

    this->_chunksAmount = chunksAmount;
    this->speed = speed;
    this->_shader = shader;
    // this->chunks.reserve(this->_chunksAmount);
    //GET LENGTH CHUNK PROPERLY !!!
    this->_chunkLength = 40.0f;

    pathPart = Object(this->_shader, "models/tubecube/tubecube.obj");

    for (int i = 0; i < this->_chunksAmount; i++){
        Object objectChunk = Object(pathPart);
        objectChunk.transform.position = vec3_forward * this->_chunkLength; // get local forward of parent
        this->chunks.push_back(objectChunk);
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
}

void PathGenerator::SwapFirstToLast(){

    RenderPipeline::ClearBuffers(this->chunks.begin()->shader, this->chunks.begin()->meshes, false);

    this->chunks.erase(this->chunks.begin());

    Object objectChunk = Object(pathPart);

    objectChunk.transform.position = vec3_forward * this->_chunkLength; // get local forward of parent
    this->chunks.push_back(objectChunk);

    //parenting
    this->chunks.begin()->transform.parent = nullptr;
    next(this->chunks.end(), -2)->transform.child = & next(this->chunks.end(), -1)->transform; 
    next(this->chunks.end(), -1)->transform.parent = & next(this->chunks.end(), -2)->transform; 
    next(this->chunks.end(), -1)->transform.child = nullptr;

    this->chunks.begin()->transform.position = this->chunks.begin()->transform.LocalToWorldPosition();
}

float PathGenerator::GetChunkLength(){
    return this->_chunkLength;
}