#include "PathGenerator.hpp"

PathGenerator::~PathGenerator(){}

PathGenerator::PathGenerator(){}

PathGenerator::PathGenerator(Shader *shader, unsigned int chunksAmount, float speed){

    if (chunksAmount < 3)
        chunksAmount = 3;

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
            this->chunks[i].transform.parent = &this->chunks[i - 1].transform;
        if (i < this->_chunksAmount - 1)
            this->chunks[i].transform.child = &this->chunks[i + 1].transform;
    }

    std::cout << "BEGIN" << std::endl;
    for (int i = 0; i < this->chunks.size(); i++){
        std::cout << "[" << i << "]  " << &this->chunks[i] << "  Z : " << this->chunks[i].transform.position.z << std::endl;
    }
    std::cout << "__________" << std::endl << std::endl;
}

void PathGenerator::SwapFirstToLast(){

    RenderPipeline::ClearBuffers(this->chunks.begin()->shader, this->chunks.begin()->meshes, false);

    this->chunks.erase(this->chunks.begin());

    Object objectChunk = Object(pathPart);

    objectChunk.transform.position = vec3_forward * this->_chunkLength; // get local forward of parent
    this->chunks.push_back(objectChunk);

    // this->chunks[0].transform.parent = nullptr;
    // this->chunks[this->_chunksAmount - 0].transform.child = nullptr;
    // this->chunks[this->_chunksAmount - 0].transform.parent = &this->chunks[this->_chunksAmount - 1].transform;
    // this->chunks[this->_chunksAmount - 1].transform.child = &this->chunks[this->_chunksAmount - 0].transform;

    for (int i = 0; i < this->_chunksAmount; i++){
        if (i != 0)
            this->chunks[i].transform.parent = &this->chunks[i - 1].transform;
        else
            this->chunks[i].transform.parent = nullptr;
        if (i < this->_chunksAmount - 1)
            this->chunks[i].transform.child = &this->chunks[i + 1].transform;
        else
            this->chunks[i].transform.child = nullptr;
    }

    this->chunks[0].transform.position = this->chunks[0].transform.LocalToWorldPosition();
}

float PathGenerator::GetChunkLength(){
    return this->_chunkLength;
}