#include "Chunk.hpp"

Chunk::~Chunk(){}

Chunk::Chunk(){}

Chunk::Chunk(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){
    //gen random chunk here ?
}

Chunk::Chunk(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){
    //gen random chunk here ?
}


void Chunk::OnColliderEnter(Collider & collider){
    std::cout << "Collision Enter : [" << this->GetTag() << " : " << this->ID << "] -> [" << collider.transform->gameObject->GetTag() << " : " << collider.transform->gameObject->ID << "]" << std::endl;
    if (this->GetTag() == "Turn" && collider.transform->GetTag() == "Player"){
        //Start Rotation
    }
}

void Chunk::OnColliderStay(Collider & collider){
    // std::cout << "Collision Stay : [" << this->ID << "] -> [" << collider.transform->gameObject->ID << "]" << std::endl;
}

void Chunk::OnColliderExit(Collider & collider){
    std::cout << "Collision Exit : [" << this->GetTag() << " : " << this->ID << "] -> [" << collider.transform->gameObject->GetTag() << " : " << collider.transform->gameObject->ID << "]" << std::endl;
    if (this->GetTag() == "Turn" && collider.transform->GetTag() == "Player"){
        //End Rotation
    }
}
