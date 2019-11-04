#include "Object.hpp"
#include "RenderPipeline.hpp"

#include <iostream>

Object::~Object(){

    // CHECK IF COLLIDERS ARE BUILD AND DESTROYED PROPERLY !!!!

    std::cout << "Destroy OBJ [" << this->ID << "]" << std::endl;
    RenderPipeline::RemoveObject((*this), false);
}

Object::Object(){
}

Object::Object(const Object & rhs, bool render, bool collide){

	this->shader = rhs.shader;
	this->transform = Transform(rhs.transform, (*this));
	this->meshes = rhs.meshes;
    this->collider = Collider(rhs.collider, this->transform, !collide);
    if (render)
        RenderPipeline::AddObject((*this));
    else
        this->ID = RenderPipeline::GenerateID();
    std::cout << "Construct Object [" << this->GetTag() << " : " << this->ID << std::endl;
}

Object::Object(Shader & shader, const char *objFile, bool render, bool collide){

    this->shader = &shader;
    vec3 minBoundPosition;
    vec3 maxBoundPosition;
	this->meshes = Loader::LoadModel(objFile, minBoundPosition, maxBoundPosition);
    this->transform = Transform(vec3_zero, (*this));
    this->collider = Collider(this->transform, minBoundPosition, maxBoundPosition, !collide);
    if (render)
        RenderPipeline::AddObject((*this));
    else
        this->ID = RenderPipeline::GenerateID();
    std::cout << "Construct Object [" << this->GetTag() << " : " << this->ID << std::endl;
}

void Object::SetShader(Shader *shader){
    this->shader = shader;
}

mat4 Object::GetModelMatrix(){
    return this->transform.modelMatrix;
}

void Object::ClearBuffers(){
    RenderPipeline::ClearBuffers((*this));
}

string Object::GetTag(){
    return this->transform.GetTag();
}

void Object::SetTag(string newTag){
    this->transform.SetTag(newTag);
}