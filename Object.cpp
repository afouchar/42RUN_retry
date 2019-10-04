#include "Object.hpp"

Object::~Object(){}

Object::Object(){

    // this->shader = Loader::DefaultShader();
    // this->meshes = Loader::DefaultMesh();
}

Object::Object(const Object& rhs){

	this->shader = rhs.shader;
	this->transform = Transform(rhs.transform);
	this->meshes = rhs.meshes;
}

Object::Object(Shader *shader, const char *objFile){

    this->shader = shader;
	this->meshes = Loader::LoadModel(objFile);
}

void Object::SetShader(Shader *shader){
    this->shader = shader;
}

mat4 Object::GetModelMatrix(){
    return this->transform.modelMatrix;
}