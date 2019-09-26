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
    // Shader _shader;
    // GLuint _VAO;

    // GLuint _VBO;
    mat4 _MVP;
    GLuint _vertexArrayID;

    GLuint AddBuffer(vector<vec3> *data);
    void BindBuffer(GLuint bufferID, int layoutLocation);

public:
    RenderPipeline();
    ~RenderPipeline();

    void GenBuffers(Object *object);
    void BindBuffers(Object *object);
    void UseProgram(Object *object);
    void Draw(Object *object);

    void SetMVP(Camera *camera, Object *object);
    // void SetVBO();
    // void ClearVBO();
    // void UseProgram();
    // void Draw();

    // void SetShader(Shader *shader);
};

#endif