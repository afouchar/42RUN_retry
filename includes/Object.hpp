#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <list>

#include "Loader.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Collider.hpp"
#include "Transform.hpp"


using namespace std;
using namespace glm;


class Object {

private:


public:
    Object();
    Object(Shader & shader, const char *objFile, bool render = true, bool collide = true);
    Object(const Object & rhs, bool render = true, bool collide = true);
    ~Object();

    Shader          *shader;
    Transform       transform;
    Collider        collider;
    vector<Mesh>    meshes;
    unsigned int    ID;

    void            SetShader(Shader *shader);
    mat4            GetModelMatrix();
    void            ClearBuffers();

    string          GetTag();
    void            SetTag(string newTag);

    //virtual methods
    virtual void    OnColliderEnter(Collider & collider){};
    virtual void    OnColliderStay(Collider & collider){};
    virtual void    OnColliderExit(Collider & collider){};
    virtual void    Update(){};
};

#endif