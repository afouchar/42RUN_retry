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
	vec3        tangent;
	vec3        biTangent;
};


struct Texture {
	GLuint      id;
	string      type;
	aiString    path;
};

struct Material {
    vec3        specular;
    vec3        diffuse;
    vec3        ambient;
    vec3        emissive;
    vec3        reflective;
    vec3        transparent;
    float       shininess;
    float       bumpScale;
};

class Mesh {

private:
    
public:
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, Material material);
    ~Mesh();

    GLuint              vertexArrayID;
    GLuint              vertexBufferID;
    GLuint              elementBufferID;

    vector<Vertex>      vertices;
    vector<GLuint>      indices;
    vector<Texture>     textures;
    Material            material;

    vec3                minVertexPosition;
    vec3                maxVertexPosition;
};

#endif