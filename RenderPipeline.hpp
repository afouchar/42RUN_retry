#ifndef RENDERPIPELINE_HPP
#define RENDERPIPELINE_HPP


#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include "Shader.hpp"
#include "Camera.hpp"
#include "Object.hpp"


class RenderPipeline {

private:

    mat4        _MVP;
    GLuint      _vertexArrayID;

    GLuint      AddBuffer(vector<vec3> *data);
    void        BindBuffer(GLuint bufferID, int layoutLocation);

public:
    RenderPipeline();
    ~RenderPipeline();

    void        SetMVP(Camera *camera, Object *object);

    void        GenBuffers(Object *object);
    void        BindBuffers(Object *object);
    void        ClearBuffers(Object *object);
    void        UseProgram(Object *object);
    void        Draw(Object *object);
    void        DrawObjectMeshes(Object *object, Mesh *mesh);
};

#endif