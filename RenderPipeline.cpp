#include "RenderPipeline.hpp"

RenderPipeline::~RenderPipeline(){}

RenderPipeline::RenderPipeline(){

    glGenVertexArrays(1, &this->_vertexArrayID);
    glBindVertexArray(this->_vertexArrayID);
}

void RenderPipeline::SetMVP(Camera *camera, Object *object){

	this->_MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix() * object->GetModelMatrix();
}

void RenderPipeline::GenBuffers(Object *object){

    object->vertexBufferID = AddBuffer(&object->vertices);
    object->colorBufferID = AddBuffer(&object->colors);
}

GLuint RenderPipeline::AddBuffer(vector<vec3> *data){

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(vec3), &(*data)[0], GL_STATIC_DRAW);
    return bufferID;
}

void RenderPipeline::BindBuffers(Object *object){

    glUniformMatrix4fv(object->shader->GetmvpID(), 1, GL_FALSE, &this->_MVP[0][0]);

    BindBuffer(object->vertexBufferID, 0);
    BindBuffer(object->colorBufferID, 1);
}

void RenderPipeline::BindBuffer(GLuint bufferID, int layoutLocation){

        glEnableVertexAttribArray(layoutLocation);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glVertexAttribPointer(
			layoutLocation,     // attribute. Must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
}

void RenderPipeline::ClearBuffers(Object *object){

    glDeleteBuffers(1, &object->vertexBufferID);
    glDeleteBuffers(1, &object->colorBufferID);
    glDeleteProgram(object->shader->GetProgramID());
    glDeleteVertexArrays(1, &this->_vertexArrayID);
}

void RenderPipeline::UseProgram(Object *object){

    glUseProgram(object->shader->GetProgramID());
}

void RenderPipeline::Draw(Object *object){

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);
    // glFrontFace(GL_CW);

    glDrawArrays(GL_TRIANGLES, 0, object->vertices.size());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}