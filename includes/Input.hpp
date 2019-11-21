#ifndef INPUT_HPP
#define INPUT_HPP


#include <glfw3.h>
#include <glm.hpp>

using namespace glm;


class Input {
private:
    Input();

    GLFWwindow      *_window;


public:
    Input(GLFWwindow & window);
    ~Input();

    float       mouseSpeed;

    void        PollEvents();
    bool        GetKeyPressed(int key);
    bool        GetKeyReleased(int key);
    vec2        GetMousePosition();
    vec2        GetMouseDirection();
    void        ResetMousePosition();
    void        Terminate();
};

#endif