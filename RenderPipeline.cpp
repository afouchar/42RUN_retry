#include "RenderPipeline.hpp"

RenderPipeline::~RenderPipeline(){}

RenderPipeline::RenderPipeline(const GLfloat model[]){

	glGenVertexArrays(1, &this->_vertexArrayID);
	glBindVertexArray(this->_vertexArrayID);

    this->_vertexBufferData = model;

	glGenBuffers(1, &this->_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->_vertexBufferData), this->_vertexBufferData, GL_STATIC_DRAW);
}

RenderPipeline::RenderPipeline(const GLfloat model[], Shader shader){

    this->_shader = shader;
	glGenVertexArrays(1, &this->_vertexArrayID);
	glBindVertexArray(this->_vertexArrayID);

    this->_vertexBufferData = model;

	glGenBuffers(1, &this->_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->_vertexBufferData), this->_vertexBufferData, GL_STATIC_DRAW);
}

void RenderPipeline::SetVBO(){

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->_vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
}

void RenderPipeline::ClearVBO(){

    // Cleanup VBO
    glDeleteBuffers(1, &this->_vertexBuffer);
    glDeleteVertexArrays(1, &this->_vertexArrayID);
    glDeleteProgram(this->_shader.GetProgramID());
}

void RenderPipeline::UseProgram(){

    // Use our shader
    glUseProgram(this->_shader.GetProgramID());
}

void RenderPipeline::Draw(){

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
    glDisableVertexAttribArray(0);
}

void RenderPipeline::SetShader(Shader shader){
    this->_shader = shader;
}