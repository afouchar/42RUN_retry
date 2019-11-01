#include "Chunk.hpp"

Chunk::~Chunk(){}

Chunk::Chunk(){}

Chunk::Chunk(const Object& rhs, bool render) : Object(rhs, render){
    //gen random chunk here ?
}

Chunk::Chunk(Shader & shader, const char *objFile, bool render) : Object(shader, objFile, render){
    //gen random chunk here ?
}


void Chunk::OnColliderEnter(Collider & collider){
    std::cout << "Collision Enter : [" << this->ID << "] -> [" << collider.transform->gameObject->ID << "]" << std::endl;
}

void Chunk::OnColliderStay(Collider & collider){
    std::cout << "Collision Stay : [" << this->ID << "] -> [" << collider.transform->gameObject->ID << "]" << std::endl;
}

void Chunk::OnColliderExit(Collider & collider){
    std::cout << "Collision Exit : [" << this->ID << "] -> [" << collider.transform->gameObject->ID << "]" << std::endl;
}
