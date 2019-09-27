#include "Window.hpp"


Window::~Window(){}

Window::Window(const char* title, vec2 size){

    CreateWindow(title, size, vec4(0.32, 0.32, 0.32, 0.0));
}

Window::Window(const char* title, vec2 size, vec4 clearColor){

    CreateWindow(title, size, clearColor);
}

void Window::CreateWindow(const char* title, vec2 size, vec4 clearColor){

	// Init GLFW
	if(!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		getchar();
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create OpenGL context
    this->_title = title;
    this->_size = size;
    this->_clearColor = clearColor;
	this->_window = glfwCreateWindow( this->_size.x, this->_size.y, this->_title.c_str(), NULL, NULL);
	if( this->_window == NULL ){
		std::cerr << "Failed to open GLFW window" << std::endl;
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(this->_window);

	// Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		getchar();
		glfwTerminate();
		exit(-1);
	}

	glClearColor(this->_clearColor.x, this->_clearColor.y, this->_clearColor.z, this->_clearColor.w);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

string Window::GetTitle(){
    return this->_title;
}

void Window::SetTitle(const char* title){
    this->_title = title;
	glfwSetWindowTitle(this->_window, this->_title.c_str());
}

vec2 Window::GetSize(){
    return this->_size;
}

vec4 Window::GetClearColor(){
    return this->_clearColor;
}

void Window::SetClearColor(vec4 clearColor){
    this->_clearColor = clearColor;
	glClearColor(this->_clearColor.x, this->_clearColor.y, this->_clearColor.z, this->_clearColor.w);
}

GLFWwindow *Window::GetWindow(){
    return this->_window;
}

void Window::Clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SwapBuffer(){
	glfwSwapBuffers(this->_window);
}

void Window::Terminate(){
	glfwTerminate();
}

bool Window::IsClosed(){
	return glfwWindowShouldClose(this->_window) == 0 ? false : true;
}