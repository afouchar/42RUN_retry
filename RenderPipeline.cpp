#include "RenderPipeline.hpp"

RenderPipeline::~RenderPipeline(){}

RenderPipeline::RenderPipeline(){

    // glGenVertexArrays(1, &this->_vertexArrayID);
    // glBindVertexArray(this->_vertexArrayID);
}

void RenderPipeline::SetMVP(Camera *camera, Object *object){

	// this->_MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix() * object->GetModelMatrix();
	object->MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix() * object->GetModelMatrix();
}

void RenderPipeline::GenVAO(Object *object){

    for (int i = 0; i < object->meshes.size(); i++) {
        glGenVertexArrays(1, &object->meshes[i].vertexArrayID);
        glBindVertexArray(object->meshes[i].vertexArrayID);
    }
}

void RenderPipeline::GenBuffers(Object *object){

    // object->vertexBufferID = AddBuffer(&object->vertices);
    // object->colorBufferID = AddBuffer(&object->colors);

    for (int i = 0; i < object->meshes.size(); i++){

        glGenBuffers(1, &object->meshes[i].elementBufferID);
        glGenBuffers(1, &object->meshes[i].vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, object->meshes[i].vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, object->meshes[i].vertices.size() * sizeof(Vertex), &object->meshes[i].vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->meshes[i].elementBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->meshes[i].indices.size() * sizeof(GLuint), &object->meshes[i].indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
    }
}

GLuint RenderPipeline::AddBuffer(vector<vec3> *data){

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(vec3), &(*data)[0], GL_STATIC_DRAW);

    return bufferID;
}

void RenderPipeline::BindBuffers(Object *object, Camera *camera, Light *light){

    object->shader->SetMatrix4fv(object->shader->GetmvpID(), &object->MVP[0][0]);
    object->shader->SetMatrix4fv(object->shader->GetModelMatrixID(), &object->GetModelMatrix()[0][0]);
    object->shader->SetMatrix4fv(object->shader->GetViewMatrixID(), &camera->GetViewMatrix()[0][0]);
    object->shader->SetMatrix4fv(object->shader->GetProjectionMatrixID(), &camera->GetProjectionMatrix()[0][0]);
    object->shader->SetFloat3(object->shader->GetLightID(), light->transform.position);

    // BindBuffer(object->vertexBufferID, 0);
    // BindBuffer(object->colorBufferID, 1);
    
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

    // glDeleteBuffers(1, &object->vertexBufferID);
    // glDeleteBuffers(1, &object->colorBufferID);
    glDeleteProgram(object->shader->GetProgramID());
    for (int i = 0; i < object->meshes.size(); i++){
        glDeleteVertexArrays(1, &object->meshes[i].vertexArrayID);
    }
}

void RenderPipeline::UseProgram(Object *object){

    glUseProgram(object->shader->GetProgramID());
}

void RenderPipeline::Draw(Object *object){

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT_AND_BACK);
    // glFrontFace(GL_CW);

    // glDrawArrays(GL_TRIANGLES, 0, object->vertices.size());

    // glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);

    for (int i = 0; i < object->meshes.size(); i++) {
        DrawObjectMeshes(object, &object->meshes[i]);
    }
    
}

void	RenderPipeline::DrawObjectMeshes(Object *object, Mesh *mesh) {

	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for(GLuint i = 0; i < mesh->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		std::string number;
		std::string name = mesh->textures[i].type;

		if(name == "texture_diffuse")
			ss << diffuseNr++;
		else if(name == "texture_specular")
			ss << specularNr++;
		number = ss.str();

		object->shader->SetFloat(object->shader->GetUniformLocation((name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
	}

	object->shader->SetFloat(object->shader->GetUniformLocation("material.shininess"), 16.0f);

	// glBindVertexArray(this->_vertexArrayID);
	glBindVertexArray(mesh->vertexArrayID);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < mesh->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}