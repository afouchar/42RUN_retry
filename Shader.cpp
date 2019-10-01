#include "Shader.hpp"

Shader::~Shader(){}

Shader::Shader(){}

Shader::Shader(const char * vertexFilePath,const char * fragmentFilePath){

	this->_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	this->_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    this->_vertexFilePath = vertexFilePath;
    this->_fragmentFilePath = fragmentFilePath;

    string vertexShaderCode = LoadShaderCode(this->_vertexFilePath);
    string fragmentShaderCode = LoadShaderCode(this->_fragmentFilePath);

    if (vertexShaderCode.empty() || fragmentShaderCode.empty()) {
        std::cerr << "Shader can't be created." << std::endl;
        delete this;
        //throw exception to exit OR use a default magenta shader instead of destroying instance !!!
    }
    else {
        std::cout << "Compiling shader : " << this->_vertexFilePath << std::endl;
        CompileShader(vertexShaderCode, this->_vertexShaderID);

        std::cout << "Compiling shader : " << fragmentFilePath << std::endl;
        CompileShader(fragmentShaderCode, this->_fragmentShaderID);

        std::cout << "Linking program..." << std::endl;
        this->_programID = LinkShaderProgram(this->_vertexShaderID, this->_fragmentShaderID);
        std::cout << "Program ID : " << this->_programID << std::endl;

        glDetachShader(this->_programID, this->_vertexShaderID);
        glDetachShader(this->_programID, this->_fragmentShaderID);

        glDeleteShader(this->_vertexShaderID);
        glDeleteShader(this->_fragmentShaderID);

        // this->_mvpID = glGetUniformLocation(this->_programID, "MVP");
        this->_mvpID = GetUniformLocation("MVP");

        this->_modelMatrixID = GetUniformLocation("M");
        this->_viewMatrixID = GetUniformLocation("V");
        this->_projectionMatrixID = GetUniformLocation("P");

        this->_lightID = GetUniformLocation("LightPosition_worldspace");
    }
}

string Shader::LoadShaderCode(const char *shaderFilePath){

	std::string shaderCode;
	std::ifstream shaderStream(shaderFilePath, std::ios::in);

    if(shaderStream.is_open()){
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
        return shaderCode;
    }else{
        std::cerr << "Cannot open file \"" << shaderFilePath << "\"" << std::endl;
        getchar();
        return string();
    }
}

void Shader::CompileShader(string shaderCode, GLuint shaderID){

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Shader
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer , NULL);
    glCompileShader(shaderID);

    // Check Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> shaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
        std::cerr << &shaderErrorMessage[0] << std::endl;
    }
}

GLuint Shader::LinkShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID){

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Link program
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check program
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        std::cerr << &ProgramErrorMessage[0] << std::endl;
    }
    return programID;
}

GLuint Shader::GetProgramID(){
    return this->_programID;
}

GLuint Shader::GetModelMatrixID(){
    return this->_modelMatrixID;
}

GLuint Shader::GetViewMatrixID(){
    return this->_viewMatrixID;
}

GLuint Shader::GetProjectionMatrixID(){
    return this->_projectionMatrixID;
}

GLuint Shader::GetmvpID(){
    return this->_mvpID;
}

GLuint Shader::GetLightID(){
    return this->_lightID;
}

GLuint Shader::GetUniformLocation(const char * variableName){
	return glGetUniformLocation(this->_programID, variableName);
}

void	Shader::SetInt(GLint id, int newValue) {
	
	glUniform1i(id, newValue);
}

void	Shader::SetFloat(GLint id, GLfloat newValue) {
	
	glUniform1f(id, newValue);
}

void	Shader::SetFloat2(GLint id, GLfloat v0, GLfloat v1) {
	
	glUniform2f(id, v0, v1);
}

void	Shader::SetFloat2(GLint id, vec2 newValues) {
	
	glUniform2f(id, newValues.x, newValues.y);
}

void	Shader::SetFloat3(GLint id, GLfloat v0, GLfloat v1, GLfloat v2) {
	
	glUniform3f(id, v0, v1, v2);
}

void	Shader::SetFloat3(GLint id, vec3 newValues) {
	
	glUniform3f(id, newValues.x, newValues.y, newValues.z);
}

void	Shader::SetFloat4(GLint id, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
	
	glUniform4f(id, v0, v1, v2, v3);
}

void	Shader::SetFloat4(GLint id, vec4 newValues) {
	
	glUniform4f(id, newValues.x, newValues.y, newValues.z, newValues.w);
}

void    Shader::SetMatrix4fv(GLint id, const GLfloat *newValue){

    glUniformMatrix4fv(id, 1, GL_FALSE, newValue);
}