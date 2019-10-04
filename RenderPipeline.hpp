#ifndef RENDERPIPELINE_HPP
#define RENDERPIPELINE_HPP


#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include "Shader.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Light.hpp"


class RenderPipeline {

private:

    void        DrawObjectMeshes(Object *object, Mesh *mesh);

public:
    RenderPipeline();
    ~RenderPipeline();

    void        GenVAO(Object *object);
    void        GenBuffers(Object *object);
    void        BindBuffers(Object *object, Camera *camera, Light *light);
    void        ClearBuffers(Object *object, bool clearTextures = true);
    void        UseProgram(Object *object);
    void        Draw(Object *object);
};

#endif