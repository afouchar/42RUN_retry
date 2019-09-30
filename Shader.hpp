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
    Shader();

    GLuint      _programID;
    GLuint      _vertexShaderID;
    GLuint      _fragmentShaderID;
    GLuint      _mvpID;
    const char  *_vertexFilePath;
    const char  *_fragmentFilePath;

public:
    Shader(const char *vertexFilePath,const char *fragmentFilePath);
    ~Shader();

    GLuint      GetProgramID();
    GLuint      GetmvpID();
    GLuint      GetUniformLocation(const char * variableName);
    void        SetInt(GLint id, int newValue);
    void        SetFloat(GLint id, GLfloat newValue);
    void        SetFloat2(GLint id, GLfloat v0, GLfloat v1);
    void        SetFloat3(GLint id, GLfloat v0, GLfloat v1, GLfloat v2);
    void        SetFloat4(GLint id, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

    string      LoadShaderCode(const char *shaderFilePath);
    void        CompileShader(string shaderCode, GLuint shaderID);
    GLuint      LinkShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
};


#endif