#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glew.h>
#include <glm.hpp>

#include <vector>

#include "Transform.hpp"
#include "Shader.hpp"

using namespace std;
using namespace glm;

class Object {

private:

    mat4 _modelMatrix;

public:
    Object();
    Object(Shader *shader);
    ~Object();

    Shader *shader;

    GLuint vertexBufferID;
    GLuint colorBufferID;

    vector<vec3> vertices;
    vector<vec3> colors;

    void SetObjTemp();

    void SetShader(Shader *shader);
    mat4 GetModelMatrix();
};

#endif