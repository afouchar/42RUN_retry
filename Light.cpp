#include "Light.hpp"

Light::~Light(){}

Light::Light(){
    transform.position = vec3_zero;
}

Light::Light(vec3 position){
    transform.position = position;
}
