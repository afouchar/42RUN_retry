#ifndef RENDERPIPELINE_HPP
#define RENDERPIPELINE_HPP


#include <glew.h>
#include <glfw3.h>
#include "Shader.hpp"


class RenderPipeline {

private:
    Shader _shader;
    GLuint _vertexArrayID;
    //Object object / model
    const GLfloat *_vertexBufferData;
    GLuint _vertexBuffer;

public:
    RenderPipeline(const GLfloat model[], Shader shader);
    RenderPipeline(const GLfloat model[]);
    ~RenderPipeline();

    void SetVBO();
    void ClearVBO();
    void UseProgram();
    void Draw();

    void SetShader(Shader shader);
};

#endif