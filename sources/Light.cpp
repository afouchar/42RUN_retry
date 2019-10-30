#include "Light.hpp"
#include "RenderPipeline.hpp"

Light::~Light(){
    RenderPipeline::RemoveLight((*this));
}

Light::Light(){

    transform.position = vec3_zero;
    this->intensity = 45.0f;
    this->color = vec3(1.0f, 1.0f, 1.0f);
    RenderPipeline::AddLight((*this));
}

Light::Light(vec3 position){

    transform.position = position;
    this->intensity = 45.0f;
    this->color = vec3(1.0f, 1.0f, 1.0f);
    RenderPipeline::AddLight((*this));
}
