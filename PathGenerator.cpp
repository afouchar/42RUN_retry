#include "PathGenerator.hpp"

PathGenerator::~PathGenerator(){}

PathGenerator::PathGenerator(){}

PathGenerator::PathGenerator(Shader *shader, unsigned int chunksAmount, float speed){

    this->_chunksAmount = chunksAmount;
    this->speed = speed;
    this->_shader = shader;
    this->chunks.reserve(this->_chunksAmount);
    //GET LENGTH CHUNK PROPERLY !!!
    this->_chunkLength = 80.0f;

    pathPart = Object(this->_shader, "models/hallway/hallway_10.obj");

    for (int i = 0; i < this->_chunksAmount; i++){
        Object objectChunk = Object(pathPart);
        objectChunk.transform.Translate((vec3_forward * (this->_chunkLength * i)));
        this->chunks.push_back(objectChunk);
    }
}

void PathGenerator::SwapFirstToLast(RenderPipeline *renderPipeline){

    renderPipeline->ClearBuffers(this->chunks.begin().base());

    this->chunks.erase(this->chunks.begin());

    Object objectChunk = Object(pathPart);

    float distance = abs(this->chunks.end()->transform.position.z) + this->_chunkLength;
    objectChunk.transform.Translate(vec3_forward * distance);

    renderPipeline->GenVAO(&objectChunk);
    renderPipeline->GenBuffers(&objectChunk);

    this->chunks.push_back(objectChunk);

}

float PathGenerator::GetChunkLength(){
    return this->_chunkLength;
}