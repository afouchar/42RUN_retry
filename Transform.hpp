#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define vec3_zero       vec3(0, 0, 0)
#define vec3_one        vec3(1, 1, 1)
#define vec3_left       vec3(-1, 0, 0)
#define vec3_right      vec3(1, 0, 0)
#define vec3_up         vec3(0, 1, 0)
#define vec3_down       vec3(0, -1, 0)
#define vec3_forward    vec3(0, 0, -1)
#define vec3_back       vec3(0, 0, 1)

using namespace glm;

class Transform {

public:
    Transform();
    Transform(vec3 pos);
    ~Transform();

    vec3        position;

    mat4        LookAt(vec3 target, vec3 up);
};

#endif