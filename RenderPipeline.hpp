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
    ~RenderPipeline();

    void SetVBO();
    void ClearVBO();
    void Draw();
};

#endif