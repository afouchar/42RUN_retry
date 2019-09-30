#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include <vector>

#include <glew.h>
#include <glm.hpp>
#include <assimp/types.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;
using namespace glm;

struct Vertex {
	vec3        position;
	vec3        normal;
	vec2        texCoords;
};


struct Texture {
	GLuint      id;
	string      type;
	aiString    path;
};


class Mesh {

private:
    
public:
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    ~Mesh();

    GLuint          vertexBufferID;
    GLuint          elementBufferID;

    vector<Vertex>     vertices;
    vector<GLuint>     indices;
    vector<Texture>    textures;
};

#endif