#include "Text.hpp"

Text::~Text(){}

Text::Text(){
    //this->shader = Loader::DefaultTextShader();
}

Text::Text(Shader *shader, Window *window, const char *texturePath){

    this->_textureID = Loader::LoadTexture(texturePath);
    this->window = window;
    this->shader = shader;
    this->_texUniformID = this->shader->GetUniformLocation("texture_text");
    this->_aspectUniformID = this->shader->GetUniformLocation("aspectRatio");

	glGenBuffers(1, &this->_vertexBufferID);
	glGenBuffers(1, &this->_UVBufferID);

    this->string = "";
    this->position = vec2(10, 10);
    this->fontSize = 12;
}

void Text::Draw(){
    Draw(this->string.c_str());
}

void Text::Draw(const char *text){

	unsigned int length = strlen(text);

	// Fill buffers
	vector<vec2> vertices;
	vector<vec2> UVs;

	for ( unsigned int i = 0; i < length; i++ ){
		
		vec2 vertex_up_left = vec2(this->position.x + i * this->fontSize, this->position.y + this->fontSize);
		vec2 vertex_up_right = vec2(this->position.x + i * this->fontSize + this->fontSize, this->position.y + this->fontSize);
		vec2 vertex_down_right = vec2(this->position.x + i * this->fontSize + this->fontSize, this->position.y);
		vec2 vertex_down_left = vec2(this->position.x + i * this->fontSize, this->position.y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f; // grid on font texture !
		float uv_y = (character / 16) / 16.0f;

		vec2 uv_up_left = vec2(uv_x, uv_y);
		vec2 uv_up_right = vec2(uv_x + 1.0f / 16.0f, uv_y);
		vec2 uv_down_right = vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		vec2 uv_down_left = vec2(uv_x, (uv_y + 1.0f / 16.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, this->_UVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(this->shader->GetProgramID());

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->_textureID);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(this->_texUniformID, 0);
    this->shader->SetFloat2(this->_aspectUniformID, this->window->GetSize());

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->_UVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void Text::Clear(){

	glDeleteBuffers(1, &this->_vertexBufferID);
	glDeleteBuffers(1, &this->_UVBufferID);
	glDeleteTextures(1, &this->_textureID);
	glDeleteProgram(this->shader->GetProgramID());
}
