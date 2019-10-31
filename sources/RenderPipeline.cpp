#include "RenderPipeline.hpp"

list<Object *> RenderPipeline::_gameObjects;
Camera *RenderPipeline::_mainCamera;
Light *RenderPipeline::_mainLight;

RenderPipeline::~RenderPipeline(){}

RenderPipeline::RenderPipeline(){}

void RenderPipeline::GenVAO(vector<Mesh> &lstMeshes){

    for (int i = 0; i < lstMeshes.size(); i++) {
        glGenVertexArrays(1, &lstMeshes[i].vertexArrayID);
        glBindVertexArray(lstMeshes[i].vertexArrayID);
    }
}

void RenderPipeline::GenBuffers(vector<Mesh> &lstMeshes){

    for (int i = 0; i < lstMeshes.size(); i++){

        glGenBuffers(1, &lstMeshes[i].elementBufferID);
        glGenBuffers(1, &lstMeshes[i].vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, lstMeshes[i].vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, lstMeshes[i].vertices.size() * sizeof(Vertex), &lstMeshes[i].vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lstMeshes[i].elementBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, lstMeshes[i].indices.size() * sizeof(GLuint), &lstMeshes[i].indices[0], GL_STATIC_DRAW);

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

void RenderPipeline::BindBuffers(Shader *shader, Camera *camera, Light *light, mat4 &objectMatrix){

    shader->SetMatrix4fv(shader->GetModelMatrixID(), &objectMatrix[0][0]);
    shader->SetMatrix4fv(shader->GetViewMatrixID(), &camera->GetViewMatrix()[0][0]);
    shader->SetMatrix4fv(shader->GetProjectionMatrixID(), &camera->GetProjectionMatrix()[0][0]);
    shader->SetFloat3(shader->GetLightPositionID(), light->transform.position);
    shader->SetFloat3(shader->GetLightColorID(), light->color);
    shader->SetFloat(shader->GetLightIntensityID(), light->intensity);
}

void RenderPipeline::ClearBuffers(Shader *shader, vector<Mesh> &lstMeshes, bool clearTextures){

    glDeleteProgram(shader->GetProgramID());
    for (int i = 0; i < lstMeshes.size(); i++){
        glDeleteVertexArrays(1, &lstMeshes[i].vertexArrayID);
        if (clearTextures){
            int texAmount = lstMeshes[i].textures.size();
            for (int j = 0; j < texAmount; j++){
                glDeleteTextures(1, &lstMeshes[i].textures[j].id);
            }
        }
    }
}

void RenderPipeline::Clear(){

    for(list<Object *>::iterator it = RenderPipeline::_gameObjects.begin(); it != RenderPipeline::_gameObjects.end(); it++) {
        RenderPipeline::RemoveObject((**it));
    }
}

void RenderPipeline::UseProgram(Shader *shader){

    glUseProgram(shader->GetProgramID());
}

void RenderPipeline::Draw(){

    for(list<Object *>::iterator it = RenderPipeline::_gameObjects.begin(); it != RenderPipeline::_gameObjects.end(); it++) {
        if ((*it)->transform.parent == nullptr)
            (*it)->transform.UpdateMatrix();

        RenderPipeline::UseProgram((*it)->shader);
        RenderPipeline::BindBuffers((*it)->shader, RenderPipeline::_mainCamera, RenderPipeline::_mainLight, (*it)->transform.modelMatrix);
        RenderPipeline::Draw((*it)->shader, (*it)->meshes);
    }
}

void RenderPipeline::Draw(Shader *shader, vector<Mesh> &lstMeshes){

    for (int i = 0; i < lstMeshes.size(); i++) {
        DrawObjectMeshes(shader, &lstMeshes[i]);
    }
    
}

void	RenderPipeline::DrawObjectMeshes(Shader *shader, Mesh *mesh) {

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

		shader->SetFloat(shader->GetUniformLocation((name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
	}

    shader->SetFloat3(shader->GetSpecularID(), mesh->material.specular);
    shader->SetFloat3(shader->GetDiffuseID(), mesh->material.diffuse);
    shader->SetFloat3(shader->GetAmbientID(), mesh->material.ambient);
    shader->SetFloat3(shader->GetEmissiveID(), mesh->material.emissive);
    shader->SetFloat3(shader->GetReflectiveID(), mesh->material.reflective);
    shader->SetFloat3(shader->GetTransparentID(), mesh->material.transparent);
    shader->SetFloat(shader->GetShininessID(), mesh->material.shininess);
    shader->SetFloat(shader->GetBumpScaleID(), mesh->material.shininess);

	glBindVertexArray(mesh->vertexArrayID);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < mesh->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void RenderPipeline::AddObject(Object & gameObject){

    RenderPipeline::_gameObjects.push_back(&gameObject);
    RenderPipeline::GenVAO(gameObject.meshes);
	RenderPipeline::GenBuffers(gameObject.meshes);
}

void RenderPipeline::RemoveObject(Object & gameObject, bool clearTextures){

    RenderPipeline::ClearBuffers(gameObject.shader, gameObject.meshes, clearTextures);
    std::list<Object *>::iterator it = std::find(RenderPipeline::_gameObjects.begin(), RenderPipeline::_gameObjects.end(), &gameObject);
    if (it != RenderPipeline::_gameObjects.end())
        RenderPipeline::_gameObjects.erase(it);
}

void RenderPipeline::AddLight(Light & mainLight){
    RenderPipeline::_mainLight = &mainLight;
}

void RenderPipeline::RemoveLight(Light & mainLight){
    RenderPipeline::_mainLight = nullptr;
}

void RenderPipeline::AddCamera(Camera & mainCamera){
    RenderPipeline::_mainCamera = &mainCamera;
}

void RenderPipeline::RemoveCamera(Camera & mainCamera){
    RenderPipeline::_mainCamera = nullptr;
}