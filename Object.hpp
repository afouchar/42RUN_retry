#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glew.h>
#include <glm.hpp>

#include <vector>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "Transform.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

using namespace std;
using namespace glm;

class Object {

private:

    void            LoadNodes(aiNode *node, const aiScene *scene, string directory);
    Mesh            LoadMesh(aiMesh *mesh, const aiScene *scene, string directory);
    Material        LoadMaterial(aiMaterial *mat);
    vector<Texture> LoadTextures(aiMaterial *mat, aiTextureType type, string typeName, string directory);
    GLint           LoadTexture(const char *path, string directory);
    vec3            ComputeTangent(vector<Vertex> vertices, int index);
    vec3            ComputeBiTangent(vector<Vertex> vertices, int index);

public:
    Object(); // should be private !!!
    Object(Shader *shader, const char *objFile);
    Object(const Object& rhs);
    ~Object();

    Shader          *shader;
    Transform       transform;
    GLuint          vertexBufferID;
    GLuint          colorBufferID;
    vector<vec3>    vertices;
    vector<vec3>    colors;
    vector<Mesh>    meshes;


    void            SetObjTemp(); //replace with objloader
    void            SetShader(Shader *shader);
    mat4            GetModelMatrix();
    void            LoadModel(string path);
};

#endif