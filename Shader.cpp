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

    // Compile Vertex Shader
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer , NULL);
    glCompileShader(shaderID);

    // Check sShader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> shaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
        std::cerr << shaderErrorMessage[0] << std::endl;
    }
}

GLuint Shader::LinkShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID){

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Link the program
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        std::cerr << ProgramErrorMessage[0] << std::endl;
    }
    return programID;
}

GLuint Shader::GetProgramID(){
    return this->_programID;
}
    
GLuint Shader::GetUniformLocation(const char * variableName){
	return glGetUniformLocation(this->_programID, variableName);
}