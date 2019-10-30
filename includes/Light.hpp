#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Transform.hpp"

using namespace glm;


class Light {

private:

public:
    Light();
    Light(vec3 position);
    // Light(vec3 position, intensity, ...);
    ~Light();

    Transform       transform;
    float           intensity;
    vec3            color;
};

#endif