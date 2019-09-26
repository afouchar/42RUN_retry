#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <stdlib.h>
#include <string.h>

#include <glew.h>

using namespace std;


class Shader {

private:
    GLuint _programID;
    GLuint _vertexShaderID;
    GLuint _fragmentShaderID;
    const char *_vertexFilePath;
    const char *_fragmentFilePath;

public:
    Shader(const char *vertexFilePath,const char *fragmentFilePath);
    Shader();
    ~Shader();

    string LoadShaderCode(const char *shaderFilePath);
    void CompileShader(string shaderCode, GLuint shaderID);
    GLuint LinkShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

    GLuint GetProgramID();
    GLuint GetUniformLocation(const char * variableName);
};


#endif