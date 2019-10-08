#include "PathGenerator.hpp"

PathGenerator::~PathGenerator(){}

PathGenerator::PathGenerator(){}

PathGenerator::PathGenerator(Shader *shader, unsigned int chunksAmount, float speed){

    if (chunksAmount < 2)
        chunksAmount = 2;

    this->_chunksAmount = chunksAmount;
    this->speed = speed;
    this->_shader = shader;
    this->chunks.reserve(this->_chunksAmount);
    //GET LENGTH CHUNK PROPERLY !!!
    this->_chunkLength = 40.0f;

    pathPart = Object(this->_shader, "models/tubecube/tubecube.obj");

    for (int i = 0; i < this->_chunksAmount; i++){
        Object objectChunk = Object(pathPart);
        objectChunk.transform.position = vec3_forward * this->_chunkLength; // get local forward of parent
        this->chunks.push_back(objectChunk);
    }

    //parenting
    for (int i = 0; i < this->_chunksAmount; i++){
        if (i != 0)
            this->chunks[i].transform.SetParent(&this->chunks[i - 1].transform);
        if (i < this->_chunksAmount - 1)
            this->chunks[i].transform.SetChild(&this->chunks[i + 1].transform);
    }
}

// void PathGenerator::SwapFirstToLast(RenderPipeline *renderPipeline){
void PathGenerator::SwapFirstToLast(){

    // renderPipeline->ClearBuffers(this->chunks.begin().base(), false);
    bool var = false;
    RenderPipeline::ClearBuffers(this->chunks.begin()->shader, this->chunks.begin()->meshes, var);

    (this->chunks.begin() + 1)->transform.position += this->chunks.begin()->transform.position;

    this->chunks.erase(this->chunks.begin());

    Object objectChunk = Object(pathPart);

    // float distance = abs(this->chunks.end()->transform.position.z) + this->_chunkLength;
    // objectChunk.transform.Translate(vec3_forward * distance);
    // objectChunk.transform.SetParent(&this->chunks.end().base()->transform);

    RenderPipeline::GenVAO(objectChunk.meshes);
    RenderPipeline::GenBuffers(objectChunk.meshes);

    this->chunks.push_back(objectChunk);

    this->chunks.begin()->transform.SetParent(nullptr);
    this->chunks.end()->transform.SetParent(&((this->chunks.end() - 1)->transform));
    this->chunks.end()->transform.SetChild(nullptr);

    this->chunks.end()->transform.position = vec3_forward * this->_chunkLength; // get local forward of parent
    this->chunks.begin()->transform.Translate(vec3_zero);
}

float PathGenerator::GetChunkLength(){
    return this->_chunkLength;
}