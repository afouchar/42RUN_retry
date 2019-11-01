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


class RenderPipeline {

private:

    static list<Object *>   _gameObjects;
    static Camera           *_mainCamera;
    static Light            *_mainLight;
    static unsigned int     _IDCount;


public:
    RenderPipeline();
    ~RenderPipeline();

    static void         GenVAO(Object & object);
    static void         GenBuffers(Object & object);
    static void         BindBuffers(Object & object, Camera & camera, Light & light);
    static void         Clear();
    static void         ClearBuffers(Object & object, bool clearTextures = true);

    static void         UseProgram(Shader & shader);

    static void         Draw();
    static void         DrawObject(Object & object);
    static void         DrawObjectMeshes(Shader & shader, Mesh & mesh);

    static void         AddObject(Object & gameObject);
    static void         RemoveObject(Object & gameObject, bool clearTextures = true);
    static void         AddLight(Light & mainLight);
    static void         RemoveLight(Light & mainLight);
    static void         AddCamera(Camera & mainCamera);
    static void         RemoveCamera(Camera & mainCamera);
};

#endif