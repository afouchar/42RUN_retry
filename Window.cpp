#include "Window.hpp"


Window::~Window(){}

Window::Window(const char* title, vec2 size){

    // this->_clearColor = vec4(0.5, 0.5, 0.5, 1);
    CreateWindow(title, size, vec4(0.5, 0.5, 0.5, 1.0));
}

Window::Window(const char* title, vec2 size, vec4 clearColor){

    CreateWindow(title, size, clearColor);
}

void Window::CreateWindow(const char* title, vec2 size, vec4 clearColor){

	// Initialise GLFW
	if(!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		getchar();
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
    this->_title = title;
    this->_size = size;
    this->_clearColor = clearColor;
	this->_window = glfwCreateWindow( this->_size.x, this->_size.y, this->_title.c_str(), NULL, NULL);
	if( this->_window == NULL ){
		std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." << std::endl;
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(this->_window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glClearColor(this->_clearColor.x, this->_clearColor.y, this->_clearColor.z, this->_clearColor.w);

}

string Window::GetTitle(){
    return this->_title;
}

void Window::SetTitle(const char* title){
    this->_title = title;
    //update window title
}

vec2 Window::GetSize(){
    return this->_size;
}

vec4 Window::GetClearColor(){
    return this->_clearColor;
}

void Window::SetClearColor(vec4 clearColor){
    this->_clearColor = clearColor;
    //udpate window
}

GLFWwindow *Window::GetWindow(){
    return this->_window;
}

void Window::ClearWindow(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SwapBufferWindow(){
	glfwSwapBuffers(this->_window);
}