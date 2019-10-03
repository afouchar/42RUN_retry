#include "Object.hpp"

Object::~Object(){}

Object::Object(){

    // this->_modelMatrix = mat4(1.0f);
    // SetObjTemp();
}

Object::Object(const Object& rhs){

	this->shader = rhs.shader;
	this->transform = Transform(rhs.transform);
	this->vertexBufferID = rhs.vertexBufferID;
	this->colorBufferID = rhs.colorBufferID;
	this->vertexBufferID = rhs.vertexBufferID;
	this->colors = rhs.colors;
	this->meshes = rhs.meshes;
}

Object::Object(Shader *shader, const char *objFile){

    this->shader = shader;
    // this->_modelMatrix = mat4(1.0f);
    // SetObjTemp();
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
	bool tangentAvailable = false;
	bool biTangentAvailable = false;

	vertices.reserve(mesh->mNumVertices);
	for(int i = 0; i < mesh->mNumVertices; i++) {

		Vertex vertex;

		vertex.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if(mesh->mTextureCoords[0])
			vertex.texCoords = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.texCoords = vec2(0.0f, 0.0f);
		if (mesh->mTangents){
			vertex.tangent = vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			tangentAvailable = true;
		}
		if (mesh->mBitangents){
			vertex.biTangent = vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
			biTangentAvailable = true;
		}
		vertices.push_back(vertex);
	}

	if (!tangentAvailable){
		for (int i = 0; i < vertices.size(); i += 3){
			vertices[i].tangent = ComputeTangent(vertices, i);
			vertices[i].tangent = ComputeTangent(vertices, i + 1);
			vertices[i].tangent = ComputeTangent(vertices, i + 2);
		}
	}

	if (!biTangentAvailable){
		for (int i = 0; i < vertices.size(); i += 3){
			vertices[i].tangent = ComputeBiTangent(vertices, i);
			vertices[i].tangent = ComputeBiTangent(vertices, i + 1);
			vertices[i].tangent = ComputeBiTangent(vertices, i + 2);
		}
	}

	indices.reserve(3 * mesh->mNumFaces);
	for(int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
    }

	// Process materials
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	// Convention (N is the index):
	// Diffuse: texture_diffuseN
	// Specular: texture_specularN
	// Normal: texture_normalN

	vector<Texture> diffuseMaps = this->LoadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	vector<Texture> specularMaps = this->LoadTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	vector<Texture> normalMaps = this->LoadTextures(material, aiTextureType_NORMALS, "texture_normal", directory);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	Material mat = LoadMaterial(material);

	return Mesh(vertices, indices, textures, mat);
}

vec3 Object::ComputeTangent(vector<Vertex> vertices, int index){

	vec3 deltaPos1 = vertices[index + 1].position - vertices[index + 0].position;
	vec3 deltaPos2 = vertices[index + 2].position - vertices[index + 0].position;
	vec2 deltaUV1 = vertices[index + 1].texCoords - vertices[index + 0].texCoords;
	vec2 deltaUV2 = vertices[index + 2].texCoords - vertices[index + 0].texCoords;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	return (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y) * r;
}

vec3 Object::ComputeBiTangent(vector<Vertex> vertices, int index){

	vec3 deltaPos1 = vertices[index + 1].position - vertices[index + 0].position;
	vec3 deltaPos2 = vertices[index + 2].position - vertices[index + 0].position;
	vec2 deltaUV1 = vertices[index + 1].texCoords - vertices[index + 0].texCoords;
	vec2 deltaUV2 = vertices[index + 2].texCoords - vertices[index + 0].texCoords;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	return (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x) * r;
}

Material Object::LoadMaterial(aiMaterial *mat){

	Material newMat;
	aiColor4D specularColor;
	aiColor4D diffuseColor;
	aiColor4D ambientColor;
	aiColor4D emissiveColor;
	aiColor4D reflectiveColor;
	aiColor4D transparentColor;
	float shininess;
	float bumpScale;

	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specularColor);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &emissiveColor);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_REFLECTIVE, &reflectiveColor);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_TRANSPARENT, &transparentColor);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);
	aiGetMaterialFloat(mat, AI_MATKEY_BUMPSCALING, &bumpScale);

	newMat.specular = vec3(specularColor.r, specularColor.g, specularColor.b);
	newMat.diffuse = vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
	newMat.ambient = vec3(ambientColor.r, ambientColor.g, ambientColor.b);
	newMat.emissive = vec3(emissiveColor.r, emissiveColor.g, emissiveColor.b);
	newMat.reflective = vec3(reflectiveColor.r, reflectiveColor.g, reflectiveColor.b);
	newMat.transparent = vec3(transparentColor.r, transparentColor.g, transparentColor.b);
	newMat.shininess = shininess;
	newMat.bumpScale = bumpScale;

	return newMat;
	// aiString matName;
	// aiGetMaterialString(mat, AI_DEFAULT_MATERIAL_NAME, 0, 0, &matName);

	// std::cout << matName.C_Str() << std::endl;
	// std::cout << "SPECULAR    : " << specularColor.r << " | " << specularColor.g << " | " << specularColor.b << std::endl;
	// std::cout << "DIFFUSE     : " << diffuseColor.r << " | " << diffuseColor.g << " | " << diffuseColor.b << std::endl;
	// std::cout << "AMBIENT     : " << ambientColor.r << " | " << ambientColor.g << " | " << ambientColor.b << std::endl;
	// std::cout << "EMISSIVE    : " << emissiveColor.r << " | " << emissiveColor.g << " | " << emissiveColor.b << std::endl;
	// std::cout << "REFLECTIVE  : " << reflectiveColor.r << " | " << reflectiveColor.g << " | " << reflectiveColor.b << std::endl;
	// std::cout << "TRANSPARENT : " << transparentColor.r << " | " << transparentColor.g << " | " << transparentColor.b << std::endl;
	// std::cout << "SHININESS   : " << shininess << std::endl;
	// std::cout << "BUMPSCLE    : " << bumpScale << std::endl;
	// std::cout << std::endl;

}

vector<Texture>	Object::LoadTextures(aiMaterial *mat, aiTextureType type, string typeName, string directory) {

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