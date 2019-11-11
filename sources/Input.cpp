#include "Input.hpp"


Input::~Input(){}

Input::Input(GLFWwindow & window){

    this->_window = &window;
    this->mouseSpeed = 0.005f;
	glfwSetInputMode(this->_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ResetMousePosition();
}

void Input::PollEvents(){
		glfwPollEvents();
}

bool Input::GetKeyPressed(int key){
    return glfwGetKey(this->_window, key ) == GLFW_PRESS ? true : false;
}

vec2 Input::GetMousePosition(){

    double xpos, ypos;
    glfwGetCursorPos(this->_window, &xpos, &ypos);

    return vec2(xpos, ypos);
}

vec2 Input::GetMouseDirection(){

    float xdir, ydir;
    int width, height;
    vec2 pos = GetMousePosition();
    glfwGetWindowSize(this->_window, &width, &height);
    xdir = this->mouseSpeed * float(width / 2 - pos.x);
    ydir = this->mouseSpeed * float(height / 2 - pos.y);

    return vec2(xdir, -ydir);
}

void Input::ResetMousePosition(){

    int width, height;
    glfwGetWindowSize(this->_window, &width, &height);
    glfwSetCursorPos(this->_window, width / 2, height / 2);
}

void Input::Terminate(){
    glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}