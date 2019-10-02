#include "RenderPipeline.hpp"

RenderPipeline::~RenderPipeline(){}

RenderPipeline::RenderPipeline(){}

void RenderPipeline::GenVAO(Object *object){

    for (int i = 0; i < object->meshes.size(); i++) {
        glGenVertexArrays(1, &object->meshes[i].vertexArrayID);
        glBindVertexArray(object->meshes[i].vertexArrayID);
    }
}

void RenderPipeline::GenBuffers(Object *object){

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

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, biTangent));
    }
}

void RenderPipeline::BindBuffers(Object *object, Camera *camera, Light *light){

    object->shader->SetMatrix4fv(object->shader->GetModelMatrixID(), &object->GetModelMatrix()[0][0]);
    object->shader->SetMatrix4fv(object->shader->GetViewMatrixID(), &camera->GetViewMatrix()[0][0]);
    object->shader->SetMatrix4fv(object->shader->GetProjectionMatrixID(), &camera->GetProjectionMatrix()[0][0]);
    object->shader->SetFloat3(object->shader->GetLightPositionID(), light->transform.position);
    object->shader->SetFloat3(object->shader->GetLightColorID(), light->color);
    object->shader->SetFloat(object->shader->GetLightIntensityID(), light->intensity);
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

    for (int i = 0; i < object->meshes.size(); i++) {
        DrawObjectMeshes(object, &object->meshes[i]);
    }
    
}

void	RenderPipeline::DrawObjectMeshes(Object *object, Mesh *mesh) {

	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;

	for(GLuint i = 0; i < mesh->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		stringstream ss;
		string number;
		string name = mesh->textures[i].type;

		if(name == "texture_diffuse")
			ss << diffuseNr++;
		else if(name == "texture_specular")
			ss << specularNr++;
        else if(name == "texture_normal")
			ss << normalNr++;
		number = ss.str();

		object->shader->SetFloat(object->shader->GetUniformLocation((name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
	}

    object->shader->SetFloat3(object->shader->GetSpecularID(), mesh->material.specular);
    object->shader->SetFloat3(object->shader->GetDiffuseID(), mesh->material.diffuse);
    object->shader->SetFloat3(object->shader->GetAmbientID(), mesh->material.ambient);
    object->shader->SetFloat3(object->shader->GetEmissiveID(), mesh->material.emissive);
    object->shader->SetFloat3(object->shader->GetReflectiveID(), mesh->material.reflective);
    object->shader->SetFloat3(object->shader->GetTransparentID(), mesh->material.transparent);
    object->shader->SetFloat(object->shader->GetShininessID(), mesh->material.shininess);
    object->shader->SetFloat(object->shader->GetBumpScaleID(), mesh->material.shininess);

	// object->shader->SetFloat(object->shader->GetUniformLocation("material.shininess"), 16.0f);

	glBindVertexArray(mesh->vertexArrayID);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < mesh->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}