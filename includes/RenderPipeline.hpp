#ifndef RENDERPIPELINE_HPP
#define RENDERPIPELINE_HPP


#include "Light.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
#include <list>

using namespace std;

// class Object;

class RenderPipeline {

private:

    static list<Object *>   _gameObjects;
    static Camera           *_mainCamera;
    static Light            *_mainLight;

    static void             DrawObjectMeshes(Shader *shader, Mesh *mesh);

public:
    RenderPipeline();
    ~RenderPipeline();

    static void         GenVAO(vector<Mesh> &lstMeshes);
    static void         GenBuffers(vector<Mesh> &lstMeshes);
    static void         BindBuffers(Shader *shader, Camera *camera, Light *light, mat4 &objectMatrix);
    static void         ClearBuffers(Shader *shader, vector<Mesh> &lstMeshes, bool clearTextures = true);
    static void         Clear();
    static void         UseProgram(Shader *shader);
    static void         Draw(Shader *shader, vector<Mesh> &lstMeshes);
    static void         Draw();
    static void         AddObject(Object & gameObject);
    static void         RemoveObject(Object & gameObject, bool clearTextures = true);
    static void         AddLight(Light & mainLight);
    static void         RemoveLight(Light & mainLight);
    static void         AddCamera(Camera & mainCamera);
    static void         RemoveCamera(Camera & mainCamera);
};

#endif