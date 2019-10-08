#ifndef RENDERPIPELINE_HPP
#define RENDERPIPELINE_HPP

#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include "Shader.hpp"
#include "Camera.hpp"
// #include "Object.hpp"
#include "Mesh.hpp"
#include "Light.hpp"

using namespace std;

class RenderPipeline {

private:

    static void        DrawObjectMeshes(Shader *shader, Mesh *mesh);

public:
    RenderPipeline();
    ~RenderPipeline();

    static void        GenVAO(vector<Mesh> &lstMeshes);
    static void        GenBuffers(vector<Mesh> &lstMeshes);
    static void        BindBuffers(Shader *shader, Camera *camera, Light *light, mat4 &objectMatrix);
    static void        ClearBuffers(Shader *shader, vector<Mesh> &lstMeshes, bool clearTextures = true);
    static void        UseProgram(Shader *shader);
    static void        Draw(Shader *shader, vector<Mesh> &lstMeshes);
};

#endif