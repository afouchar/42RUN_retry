#include "Mesh.hpp"

Mesh::~Mesh(){}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, Material material){

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->material = material;
}