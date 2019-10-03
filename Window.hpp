#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <iostream>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

using namespace std;
using namespace glm;


class Window {

private:
    string      _title;
    vec2        _size;
    vec4        _clearColor;
    GLFWwindow* _window;
    float       _ratio;

    void        CreateWindow(const char* title, vec2 size, vec4 clearColor);


public:
    Window(const char* title, vec2 size);
    Window(const char* title, vec2 size, vec4 clearColor);
    ~Window();

    string      GetTitle();
    void        SetTitle(const char* title);
    vec2        GetSize();
    float       GetRatio();
    vec4        GetClearColor();
    void        SetClearColor(vec4 clearColor);
    GLFWwindow  *GetWindow();

    void        Clear();
    void        SwapBuffer();
    void        Terminate();
    bool        IsClosed();

};

#endif