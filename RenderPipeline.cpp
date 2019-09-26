#include "RenderPipeline.hpp"

RenderPipeline::~RenderPipeline(){}

RenderPipeline::RenderPipeline(){

    glGenVertexArrays(1, &this->_vertexArrayID);
    glBindVertexArray(this->_vertexArrayID);
}

void RenderPipeline::SetMVP(Camera *camera, Object *object){

	// Our ModelViewProjection : multiplication of our 3 matrices
    // Remember, matrix multiplication is the other way around
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
	glBufferData(GL_ARRAY_BUFFER, (*data).size() * sizeof(vec3), &(*data)[0], GL_STATIC_DRAW);
    return bufferID;
}

void RenderPipeline::BindBuffers(Object *object){

    BindBuffer(object->vertexBufferID, 0);
    BindBuffer(object->colorBufferID, 1);
}

void RenderPipeline::BindBuffer(GLuint bufferID, int layoutLocation){

        glEnableVertexAttribArray(layoutLocation);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glVertexAttribPointer(
			layoutLocation,                  // attribute. Must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
}

void RenderPipeline::UseProgram(Object *object){

    // Use our shader
    glUseProgram(object->shader->GetProgramID());
}

void RenderPipeline::Draw(Object *object){

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);
    // glFrontFace(GL_CW); 
    glUniformMatrix4fv(object->shader->GetUniformLocation("MVP"), 1, GL_FALSE, &this->_MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, object->vertices.size()); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

// void RenderPipeline::SetVBO(){

// 	glGenVertexArrays(1, &this->_VAO);
// 	glGenBuffers(1, &this->_VBO);
// 	glBindVertexArray(this->_VAO);

// 	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
// 	glBufferData(GL_ARRAY_BUFFER, test.size() * sizeof(this->_vertexBufferData), &this->test[0], GL_STATIC_DRAW);


//     // 1rst attribute buffer : vertices
//     glVertexAttribPointer(
//         0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//         3,                  // size
//         GL_FLOAT,           // type
//         GL_FALSE,           // normalized?
//         0,                  // stride
//         (void*)0            // array buffer offset
//     );
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);

//     glBindVertexArray(0);
// }

// void RenderPipeline::ClearVBO(){

//     // Cleanup VBO
//     glDeleteBuffers(1, &this->_VBO);
//     glDeleteVertexArrays(1, &this->_VAO);
//     glDeleteProgram(this->_shader.GetProgramID());
// }

// void RenderPipeline::UseProgram(){

//     // Use our shader
//     glUseProgram(this->_shader.GetProgramID());
// }

// void RenderPipeline::Draw(){

//     // glEnable(GL_CULL_FACE);
//     // glCullFace(GL_FRONT_AND_BACK);
//     // glFrontFace(GL_CW); 
//     glUniformMatrix4fv(this->_shader.GetUniformLocation("MVP"), 1, GL_FALSE, &this->_MVP[0][0]);

//     glBindVertexArray(this->_VAO);

//     // Draw the triangle !
//     glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
//     // glDisableVertexAttribArray(0);
// }

// void RenderPipeline::SetShader(Shader *shader){
//     this->_shader = (*shader);
// }