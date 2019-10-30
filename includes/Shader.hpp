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
// #include <string.h>
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

    GLuint      _modelMatrixID;
    GLuint      _viewMatrixID;
    GLuint      _projectionMatrixID;

    GLuint      _lightPositionID;
    GLuint      _lightColorID;
    GLuint      _lightIntensityID;

    GLuint      _specularID;
    GLuint      _diffuseID;
    GLuint      _ambientID;
    GLuint      _emissiveID;
    GLuint      _reflectiveID;
    GLuint      _transparentID;
    GLuint      _shininessID;
    GLuint      _bumpScaleID;

    const char  *_vertexFilePath;
    const char  *_fragmentFilePath;

public:
    Shader(const char *vertexFilePath,const char *fragmentFilePath);
    ~Shader();

    GLuint      GetProgramID();
    GLuint      GetModelMatrixID();
    GLuint      GetViewMatrixID();
    GLuint      GetProjectionMatrixID();
    GLuint      GetLightPositionID();
    GLuint      GetLightColorID();
    GLuint      GetLightIntensityID();
    GLuint      GetSpecularID();
    GLuint      GetDiffuseID();
    GLuint      GetAmbientID();
    GLuint      GetEmissiveID();
    GLuint      GetReflectiveID();
    GLuint      GetTransparentID();
    GLuint      GetShininessID();
    GLuint      GetBumpScaleID();
    GLuint      GetUniformLocation(const char * variableName);
    void        SetInt(GLint id, int newValue);
    void        SetFloat(GLint id, GLfloat newValue);
    void        SetFloat2(GLint id, GLfloat v0, GLfloat v1);
    void        SetFloat2(GLint id, vec2 newValues);
    void        SetFloat3(GLint id, GLfloat v0, GLfloat v1, GLfloat v2);
    void        SetFloat3(GLint id, vec3 newValues);
    void        SetFloat4(GLint id, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void        SetFloat4(GLint id, vec4 newValues);
    void        SetMatrix3fv(GLint id, const GLfloat *newValue);
    void        SetMatrix4fv(GLint id, const GLfloat *newValue);

    string      LoadShaderCode(const char *shaderFilePath);
    void        CompileShader(string shaderCode, GLuint shaderID);
    GLuint      LinkShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
};


#endif