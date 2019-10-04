#ifndef LOADER_HPP
#define LOADER_HPP

#include <glew.h>
#include <glm.hpp>

#include <vector>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "Mesh.hpp"

using namespace std;
using namespace glm;

class Loader {

private:

public:
    Loader();
    ~Loader();

    static vector<Mesh>     LoadModel(string path);
    static void             LoadNodes(vector<Mesh> *meshesNode, aiNode *node, const aiScene *scene, string directory);
    static Mesh             LoadMesh(aiMesh *mesh, const aiScene *scene, string directory);
    static Material         LoadMaterial(aiMaterial *mat);
    static vector<Texture>  LoadTextures(aiMaterial *mat, aiTextureType type, string typeName, string directory);
    static GLint            LoadTexture(const char *path, string directory);
    static GLint            LoadTexture(string fullPath);
    static vec3             ComputeTangent(vector<Vertex> vertices, int index);
    static vec3             ComputeBiTangent(vector<Vertex> vertices, int index);
};

#endif