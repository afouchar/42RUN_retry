#ifndef INPUT_HPP
#define INPUT_HPP


#include <glfw3.h>
#include <glm.hpp>

using namespace glm;


class Input {
private:
    Input();

    GLFWwindow      *_window;
    double          _lastTime;


public:
    Input(GLFWwindow & window);
    ~Input();

    float       speed;
    float       mouseSpeed;

    void        PollEvents();
    bool        GetKeyPressed(int key);
    vec2        GetMousePosition();
    vec2        GetMouseDirection();
    void        ResetMousePosition();
    float       DeltaTime();
    void        Terminate();
};

#endif