#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <list>

#include "Loader.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Collider.hpp"
#include "Transform.hpp"

// #include <glew.h>
// #include <glm.hpp>
// #include <vector>

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
    Collider        collider;
    vector<Mesh>    meshes;

    // void            Draw(Camera *camera, Light *light);
    void            SetShader(Shader *shader);
    mat4            GetModelMatrix();
    void            ClearBuffers();

    string          GetTag();
    void            SetTag(string newTag);

    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider){};
    virtual void    OnColliderStay(Collider & collider){};
    virtual void    OnColliderExit(Collider & collider){};
};

#endif