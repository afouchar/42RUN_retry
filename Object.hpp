#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glew.h>
#include <glm.hpp>

#include <vector>

#include "Transform.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Loader.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "RenderPipeline.hpp"

using namespace std;
using namespace glm;

class Object {

private:

public:
    Object();
    Object(Shader *shader, const char *objFile);
    Object(const Object& rhs);
    ~Object();

    Shader          *shader;
    Transform       transform;
    vector<Mesh>    meshes;

    void            Draw(Camera *camera, Light *light);
    void            SetShader(Shader *shader);
    mat4            GetModelMatrix();
    void            ClearBuffers();

    string          GetTag();
    void            SetTag(string newTag);
};

#endif