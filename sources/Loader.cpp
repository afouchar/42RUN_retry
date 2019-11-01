#include "Loader.hpp"


vector<Mesh> Loader::LoadModel(string path){

	Assimp::Importer import;
	string directory = path.substr(0, path.find_last_of('/'));
	vector<Mesh> meshesNode;

	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		std::cerr << import.GetErrorString() << std::endl;
	else
		LoadNodes(meshesNode, (*scene->mRootNode), (*scene), directory);

	return meshesNode;
}

vector<Mesh> Loader::LoadModel(string path, vec3 & minBoundPosition, vec3 & maxBoundPosition){

	vector<Mesh> meshesNode = LoadModel(path);

	minBoundPosition = vec3(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());
	maxBoundPosition = vec3(numeric_limits<float>::min(), numeric_limits<float>::min(), numeric_limits<float>::min());

	for (int i = 0; i < meshesNode.size(); i++){
		minBoundPosition = MinVec3(meshesNode[i].minVertexPosition, minBoundPosition);
		maxBoundPosition = MaxVec3(meshesNode[i].maxVertexPosition, maxBoundPosition);
	}
	return meshesNode;
}

void	Loader::LoadNodes(vector<Mesh> & meshesNode, aiNode & node, const aiScene & scene, string directory) {

	for(GLuint i = 0; i < node.mNumMeshes; i++) {
		aiMesh *aimesh = scene.mMeshes[node.mMeshes[i]];
		Mesh mesh = LoadMesh((*aimesh), scene, directory);
		meshesNode.push_back(mesh);
	}
	for(GLuint i = 0; i < node.mNumChildren; i++)
		LoadNodes(meshesNode, (*node.mChildren[i]), scene, directory);
}

Mesh	Loader::LoadMesh(aiMesh & mesh, const aiScene & scene, string directory) {

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	bool tangentAvailable = false;
	bool biTangentAvailable = false;
	vec3 meshMinVertPos = vec3(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());
	vec3 meshMaxVertPos = vec3(numeric_limits<float>::min(), numeric_limits<float>::min(), numeric_limits<float>::min());

	vertices.reserve(mesh.mNumVertices);
	for(int i = 0; i < mesh.mNumVertices; i++) {

		Vertex vertex;

		vertex.position = vec3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z);
		vertex.normal = vec3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z);
		if(mesh.mTextureCoords[0])
			vertex.texCoords = vec2(mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y);
		else
			vertex.texCoords = vec2(0.0f, 0.0f);
		if (mesh.mTangents){
			vertex.tangent = vec3(mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z);
			tangentAvailable = true;
		}
		if (mesh.mBitangents){
			vertex.biTangent = vec3(mesh.mBitangents[i].x, mesh.mBitangents[i].y, mesh.mBitangents[i].z);
			biTangentAvailable = true;
		}
		vertices.push_back(vertex);
		meshMinVertPos = MinVec3(vertex.position, meshMinVertPos);
		meshMaxVertPos = MaxVec3(vertex.position, meshMaxVertPos);

	}

	if (!tangentAvailable){
		for (int i = 0; i + 3 < vertices.size(); i += 3){
			vertices[i].tangent = ComputeTangent(vertices, i);
			vertices[i].tangent = ComputeTangent(vertices, i + 1);
			vertices[i].tangent = ComputeTangent(vertices, i + 2);
		}
	}

	if (!biTangentAvailable){
		for (int i = 0; i + 3 < vertices.size(); i += 3){
			vertices[i].tangent = ComputeBiTangent(vertices, i);
			vertices[i].tangent = ComputeBiTangent(vertices, i + 1);
			vertices[i].tangent = ComputeBiTangent(vertices, i + 2);
		}
	}

	indices.reserve(3 * mesh.mNumFaces);
	for(int i = 0; i < mesh.mNumFaces; i++) {
		aiFace face = mesh.mFaces[i];
		for(int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
    }

	// Process materials
	aiMaterial *material = scene.mMaterials[mesh.mMaterialIndex];
	// Convention (N is the index):
	// Diffuse: texture_diffuseN
	// Specular: texture_specularN
	// Normal: texture_normalN

	vector<Texture> diffuseMaps = LoadTextures((*material), aiTextureType_DIFFUSE, "texture_diffuse", directory);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	vector<Texture> specularMaps = LoadTextures((*material), aiTextureType_SPECULAR, "texture_specular", directory);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	vector<Texture> normalMaps = LoadTextures((*material), aiTextureType_NORMALS, "texture_normal", directory);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	Material mat = LoadMaterial((*material));

	Mesh outMesh = Mesh(vertices, indices, textures, mat);
	outMesh.minVertexPosition = meshMinVertPos;
	outMesh.maxVertexPosition = meshMaxVertPos;
	return outMesh;
}

vec3 Loader::MinVec3(vec3 lhs, vec3 rhs){

	vec3 min = vec3(0, 0, 0);
	min.x = lhs.x < rhs.x ? lhs.x : rhs.x;
	min.y = lhs.y < rhs.y ? lhs.y : rhs.y;
	min.z = lhs.z < rhs.z ? lhs.z : rhs.z;

	return min;
}

vec3 Loader::MaxVec3(vec3 lhs, vec3 rhs){

	vec3 max = vec3(0, 0, 0);
	max.x = lhs.x > rhs.x ? lhs.x : rhs.x;
	max.y = lhs.y > rhs.y ? lhs.y : rhs.y;
	max.z = lhs.z > rhs.z ? lhs.z : rhs.z;

	return max;
}

vec3 Loader::ComputeTangent(vector<Vertex> vertices, int index){

	vec3 deltaPos1 = vertices[index + 1].position - vertices[index + 0].position;
	vec3 deltaPos2 = vertices[index + 2].position - vertices[index + 0].position;
	vec2 deltaUV1 = vertices[index + 1].texCoords - vertices[index + 0].texCoords;
	vec2 deltaUV2 = vertices[index + 2].texCoords - vertices[index + 0].texCoords;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	return (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y) * r;
}

vec3 Loader::ComputeBiTangent(vector<Vertex> vertices, int index){

	vec3 deltaPos1 = vertices[index + 1].position - vertices[index + 0].position;
	vec3 deltaPos2 = vertices[index + 2].position - vertices[index + 0].position;
	vec2 deltaUV1 = vertices[index + 1].texCoords - vertices[index + 0].texCoords;
	vec2 deltaUV2 = vertices[index + 2].texCoords - vertices[index + 0].texCoords;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	return (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x) * r;
}

Material Loader::LoadMaterial(aiMaterial & mat){

	Material newMat;
	aiColor4D specularColor;
	aiColor4D diffuseColor;
	aiColor4D ambientColor;
	aiColor4D emissiveColor;
	aiColor4D reflectiveColor;
	aiColor4D transparentColor;
	float shininess;
	float bumpScale;

	aiGetMaterialColor(&mat, AI_MATKEY_COLOR_SPECULAR, &specularColor);
	aiGetMaterialColor(&mat, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
	aiGetMaterialColor(&mat, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
	aiGetMaterialColor(&mat, AI_MATKEY_COLOR_EMISSIVE, &emissiveColor);
	aiGetMaterialColor(&mat, AI_MATKEY_COLOR_REFLECTIVE, &reflectiveColor);
	aiGetMaterialColor(&mat, AI_MATKEY_COLOR_TRANSPARENT, &transparentColor);
	aiGetMaterialFloat(&mat, AI_MATKEY_SHININESS, &shininess);
	aiGetMaterialFloat(&mat, AI_MATKEY_BUMPSCALING, &bumpScale);

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

vector<Texture>	Loader::LoadTextures(aiMaterial & mat, aiTextureType type, string typeName, string directory) {

	vector<Texture> array_textures;
	unsigned int textureCount = mat.GetTextureCount(type);

	array_textures.reserve(textureCount);
	for(GLuint i = 0; i < textureCount; i++) {

		aiString str;
		mat.GetTexture(type, i, &str);
		GLboolean skip = false;

		Texture temp_tex;
		temp_tex.id = LoadTexture(str.C_Str(), directory);
		temp_tex.type = typeName;
		temp_tex.path = str;
		array_textures.push_back(temp_tex);
	}
	return array_textures;
}
GLint Loader::LoadTexture(const char *path, string directory){

	string fullPath(path);
	fullPath = directory + '/' + fullPath;
    return LoadTexture(fullPath);
}

GLint Loader::LoadTexture(string fullPath) {

	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	success = ilLoadImage(fullPath.c_str());

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