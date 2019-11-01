#include "RenderPipeline.hpp"

//STATIC VARIABLES DEFINITIONS
list<Object *> RenderPipeline::_gameObjects;
Camera *RenderPipeline::_mainCamera;
Light *RenderPipeline::_mainLight;
unsigned int RenderPipeline::_IDCount = 0;


RenderPipeline::~RenderPipeline(){}

RenderPipeline::RenderPipeline(){}

void RenderPipeline::GenVAO(Object & object){

    for (int i = 0; i < object.meshes.size(); i++) {
        glGenVertexArrays(1, &object.meshes[i].vertexArrayID);
        glBindVertexArray(object.meshes[i].vertexArrayID);
    }
}

void RenderPipeline::GenBuffers(Object & object){

    for (int i = 0; i < object.meshes.size(); i++){

        glGenBuffers(1, &object.meshes[i].elementBufferID);
        glGenBuffers(1, &object.meshes[i].vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, object.meshes[i].vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, object.meshes[i].vertices.size() * sizeof(Vertex), &object.meshes[i].vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.meshes[i].elementBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.meshes[i].indices.size() * sizeof(GLuint), &object.meshes[i].indices[0], GL_STATIC_DRAW);

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

void RenderPipeline::BindBuffers(Object & object, Camera & camera, Light & light){

    object.shader->SetMatrix4fv(object.shader->GetModelMatrixID(), &object.transform.modelMatrix[0][0]);
    object.shader->SetMatrix4fv(object.shader->GetViewMatrixID(), &camera.GetViewMatrix()[0][0]);
    object.shader->SetMatrix4fv(object.shader->GetProjectionMatrixID(), &camera.GetProjectionMatrix()[0][0]);
    object.shader->SetFloat3(object.shader->GetLightPositionID(), light.transform.position);
    object.shader->SetFloat3(object.shader->GetLightColorID(), light.color);
    object.shader->SetFloat(object.shader->GetLightIntensityID(), light.intensity);
}

void RenderPipeline::ClearBuffers(Object & object, bool clearTextures){

    glDeleteProgram(object.shader->GetProgramID());
    for (int i = 0; i < object.meshes.size(); i++){
        glDeleteVertexArrays(1, &object.meshes[i].vertexArrayID);
        if (clearTextures){
            int texAmount = object.meshes[i].textures.size();
            for (int j = 0; j < texAmount; j++){
                glDeleteTextures(1, &object.meshes[i].textures[j].id);
            }
        }
    }
}

void RenderPipeline::Clear(){

    for(list<Object *>::iterator it = RenderPipeline::_gameObjects.begin(); it != RenderPipeline::_gameObjects.end(); it++) {
        RenderPipeline::RemoveObject((**it));
    }
}

void RenderPipeline::UseProgram(Shader & shader){

    glUseProgram(shader.GetProgramID());
}

void RenderPipeline::Draw(){

    for(list<Object *>::iterator it = RenderPipeline::_gameObjects.begin(); it != RenderPipeline::_gameObjects.end(); it++) {
        if ((*it)->transform.parent == nullptr)
            (*it)->transform.UpdateMatrix();

        RenderPipeline::UseProgram((*(*it)->shader));
        RenderPipeline::BindBuffers((**it), (*RenderPipeline::_mainCamera), (*RenderPipeline::_mainLight));
        RenderPipeline::DrawObject((**it));
    }
}

void RenderPipeline::DrawObject(Object & object){

    for (int i = 0; i < object.meshes.size(); i++) {
        DrawObjectMeshes((*object.shader), object.meshes[i]);
    }
}

void	RenderPipeline::DrawObjectMeshes(Shader & shader, Mesh & mesh) {

	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;

	for(GLuint i = 0; i < mesh.textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		stringstream ss;
		string number;
		string name = mesh.textures[i].type;

		if(name == "texture_diffuse")
			ss << diffuseNr++;
		else if(name == "texture_specular")
			ss << specularNr++;
        else if(name == "texture_normal")
			ss << normalNr++;
		number = ss.str();

		shader.SetFloat(shader.GetUniformLocation((name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
	}

    shader.SetFloat3(shader.GetSpecularID(), mesh.material.specular);
    shader.SetFloat3(shader.GetDiffuseID(), mesh.material.diffuse);
    shader.SetFloat3(shader.GetAmbientID(), mesh.material.ambient);
    shader.SetFloat3(shader.GetEmissiveID(), mesh.material.emissive);
    shader.SetFloat3(shader.GetReflectiveID(), mesh.material.reflective);
    shader.SetFloat3(shader.GetTransparentID(), mesh.material.transparent);
    shader.SetFloat(shader.GetShininessID(), mesh.material.shininess);
    shader.SetFloat(shader.GetBumpScaleID(), mesh.material.shininess);

	glBindVertexArray(mesh.vertexArrayID);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < mesh.textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void RenderPipeline::AddObject(Object & gameObject){

    RenderPipeline::_gameObjects.push_back(&gameObject);
    RenderPipeline::GenVAO(gameObject);
	RenderPipeline::GenBuffers(gameObject);
    gameObject.ID = RenderPipeline::_IDCount;
    RenderPipeline::_IDCount++;
}

void RenderPipeline::RemoveObject(Object & gameObject, bool clearTextures){

    RenderPipeline::ClearBuffers(gameObject, clearTextures);
    std::list<Object *>::iterator it = std::find(RenderPipeline::_gameObjects.begin(), RenderPipeline::_gameObjects.end(), &gameObject);
    if (it != RenderPipeline::_gameObjects.end()){
        RenderPipeline::_gameObjects.erase(it);
    }
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