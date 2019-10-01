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
#include <glm.hpp>

using namespace std;
using namespace glm;


class Shader {

private:
    Shader();

    GLuint      _programID;
    GLuint      _vertexShaderID;
    GLuint      _fragmentShaderID;
    GLuint      _mvpID;
    GLuint      _modelMatrixID;
    GLuint      _viewMatrixID;
    GLuint      _projectionMatrixID;
    GLuint      _lightID;
    const char  *_vertexFilePath;
    const char  *_fragmentFilePath;

public:
    Shader(const char *vertexFilePath,const char *fragmentFilePath);
    ~Shader();

    GLuint      GetProgramID();
    GLuint      GetModelMatrixID();
    GLuint      GetViewMatrixID();
    GLuint      GetProjectionMatrixID();
    GLuint      GetmvpID();
    GLuint      GetLightID();
    GLuint      GetUniformLocation(const char * variableName);
    void        SetInt(GLint id, int newValue);
    void        SetFloat(GLint id, GLfloat newValue);
    void        SetFloat2(GLint id, GLfloat v0, GLfloat v1);
    void        SetFloat2(GLint id, vec2 newValues);
    void        SetFloat3(GLint id, GLfloat v0, GLfloat v1, GLfloat v2);
    void        SetFloat3(GLint id, vec3 newValues);
    void        SetFloat4(GLint id, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void        SetFloat4(GLint id, vec4 newValues);
    void        SetMatrix4fv(GLint id, const GLfloat *newValue);

    string      LoadShaderCode(const char *shaderFilePath);
    void        CompileShader(string shaderCode, GLuint shaderID);
    GLuint      LinkShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
};


#endif