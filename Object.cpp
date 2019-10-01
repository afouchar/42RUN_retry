#include "Object.hpp"

Object::~Object(){}

Object::Object(){

    // this->_modelMatrix = mat4(1.0f);
    SetObjTemp();
}

Object::Object(Shader *shader, const char *objFile){

    this->shader = shader;
    // this->_modelMatrix = mat4(1.0f);
    SetObjTemp();
	LoadModel(objFile);
}

void Object::SetObjTemp(){
    	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	static const GLfloat g_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

    int verticesLength = (sizeof(g_vertex_buffer_data) / sizeof(g_vertex_buffer_data[0]));
    for (int i = 0; i < verticesLength; i += 3){
        glm::vec3 vertex = glm::vec3(g_vertex_buffer_data[i], g_vertex_buffer_data[i + 1], g_vertex_buffer_data[i + 2]);
        this->vertices.push_back(vertex);
    }

    int colorsLength = (sizeof(g_color_buffer_data) / sizeof(g_color_buffer_data[0]));
    for (int i = 0; i < colorsLength; i += 3){
        glm::vec3 color = glm::vec3(g_color_buffer_data[i], g_color_buffer_data[i + 1], g_color_buffer_data[i + 2]);
        this->colors.push_back(color);
    }
}

void Object::SetShader(Shader *shader){
    this->shader = shader;
}

mat4 Object::GetModelMatrix(){
    // return this->_modelMatrix;
    return this->transform.modelMatrix;// * this->transform.rotation;
}

void Object::LoadModel(string path){

	Assimp::Importer import;

	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << import.GetErrorString() << std::endl;
		return;
	}
	string directory = path.substr(0, path.find_last_of('/'));
	LoadNodes(scene->mRootNode, scene, directory);
}

void	Object::LoadNodes(aiNode *node, const aiScene *scene, string directory) {

	for(GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh *aimesh = scene->mMeshes[node->mMeshes[i]];
		Mesh mesh = LoadMesh(aimesh, scene, directory);
		this->meshes.push_back(mesh);
	}
	for(GLuint i = 0; i < node->mNumChildren; i++)
		LoadNodes(node->mChildren[i], scene, directory);
}

Mesh	Object::LoadMesh(aiMesh *mesh, const aiScene *scene, string directory) {

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	vertices.reserve(mesh->mNumVertices);
	for(GLuint i = 0; i < mesh->mNumVertices; i++) {

		Vertex vertex;

		vertex.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if(mesh->mTextureCoords[0])
			vertex.texCoords = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.texCoords = vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	indices.reserve(3 * mesh->mNumFaces);
	for(GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
    }

	// Process materials
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
	// Same applies to other texture as the following list summarizes:
	// Diffuse: texture_diffuseN
	// Specular: texture_specularN
	// Normal: texture_normalN

	std::vector<Texture> diffuseMaps = this->LoadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = this->LoadMaterial(material, aiTextureType_SPECULAR, "texture_specular", directory);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	return Mesh(vertices, indices, textures);
}

vector<Texture>	Object::LoadMaterial(aiMaterial *mat, aiTextureType type, string typeName, string directory) {

	vector<Texture> array_textures;
	unsigned int textureCount = mat->GetTextureCount(type);

	array_textures.reserve(textureCount);
	for(GLuint i = 0; i < textureCount; i++) {

		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;

		Texture temp_tex;
		temp_tex.id = LoadTexture(str.C_Str(), directory);
		temp_tex.type = typeName;
		temp_tex.path = str;
		array_textures.push_back(temp_tex);
	}
	return array_textures;
}

GLint Object::LoadTexture(const char *path, string directory) {

	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	string filename(path);
	filename = directory + '/' + filename;

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	success = ilLoadImage(filename.c_str());

	if (success) {
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!success) {
			error = ilGetError();
			std::cerr << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
			return -1;
		}

		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 ilGetInteger(IL_IMAGE_FORMAT),
					 ilGetInteger(IL_IMAGE_WIDTH),
					 ilGetInteger(IL_IMAGE_HEIGHT),
					 0,
					 ilGetInteger(IL_IMAGE_FORMAT),
					 GL_UNSIGNED_BYTE,
					 ilGetData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		error = ilGetError();
		std::cerr << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		return -1;
	}
	ilDeleteImages(1, &imageID);
	return textureID;
}