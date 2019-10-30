#include "Object.hpp"
#include "RenderPipeline.hpp"

#include <iostream>

Object::~Object(){
    // this->collider.~Collider();
    std::cout << "Destroy" << std::endl;
    RenderPipeline::RemoveObject((*this), false);
}

Object::Object(){
}

Object::Object(const Object& rhs){

	this->shader = rhs.shader;
	this->transform = Transform(rhs.transform, (*this));
	this->meshes = rhs.meshes;
    this->collider = Collider(rhs.collider, this->transform);
    RenderPipeline::AddObject((*this));

    // RenderPipeline::GenVAO(this->meshes);
	// RenderPipeline::GenBuffers(this->meshes);
}

Object::Object(Shader *shader, const char *objFile){

    this->shader = shader;
	this->meshes = Loader::LoadModel(objFile);
    this->transform = Transform(vec3_zero, (*this));
    this->collider = Collider(this->transform, Loader::minVertexPosition, Loader::maxVertexPosition);
    RenderPipeline::AddObject((*this));

    // RenderPipeline::GenVAO(this->meshes);
	// RenderPipeline::GenBuffers(this->meshes);
}

// void Object::Draw(Camera *camera, Light *light){
    
//     if (this->transform.parent == nullptr)
//         this->transform.UpdateMatrix();

//     RenderPipeline::UseProgram(this->shader);
//     RenderPipeline::BindBuffers(this->shader, camera, light, this->transform.modelMatrix);
//     RenderPipeline::Draw(this->shader, this->meshes);
// }

void Object::SetShader(Shader *shader){
    this->shader = shader;
}

mat4 Object::GetModelMatrix(){
    return this->transform.modelMatrix;
}

void Object::ClearBuffers(){
    RenderPipeline::ClearBuffers(this->shader, this->meshes);
}

string Object::GetTag(){
    return this->transform.GetTag();
}

void Object::SetTag(string newTag){
    this->transform.SetTag(newTag);
}