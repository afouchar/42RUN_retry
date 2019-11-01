#ifndef TEXT_HPP
#define TEXT_HPP

#include <glew.h>
#include <glm.hpp>

#include <vector>

#include "Window.hpp"
#include "Shader.hpp"
#include "Loader.hpp"

using namespace std;
using namespace glm;

class Text {

private:

    GLuint                  _textureID;
    GLuint                  _vertexBufferID;
    GLuint                  _UVBufferID;
    // GLuint                  _shaderID;
    GLuint                  _texUniformID;
    GLuint                  _aspectUniformID;

    // GLuint              vertexArrayID;
    // GLuint              vertexBufferID;
    // GLuint              elementBufferID;

public:
    Text();
    Text(Shader & shader, Window & window, const char *texturePath);
    ~Text();

    Shader                  *shader;
    Window                  *window;
    string                  string;
    vec2                    position;
    unsigned int            fontSize;

    void                    Draw();
    void                    Draw(const char *text);
    void                    Clear();
};

#endif