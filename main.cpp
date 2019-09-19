#include <stdio.h>
#include <stdlib.h>

#include "Window.hpp"
#include "Shader.hpp"
#include "RenderPipeline.hpp"

//clang++ *.cpp -I *.hpp -I /Users/afouchar/.brew/Cellar/glfw/3.3/include/GLFW/ -I /Users/afouchar/.brew/Cellar/glm/0.9.9.5/include/glm/ -I /Users/afouchar/.brew/Cellar/glew/2.1.0/include/GL/ /Users/afouchar/.brew/lib/libglfw.3.3.dylib /Users/afouchar/.brew/Cellar/glew/2.1.0/lib/libGLEW.2.1.0.dylib -framework OpenGL


int main( void ) {

	Window window = Window("TEST", vec2(1024, 768));

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window.GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	const static GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	Shader shader = Shader("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	RenderPipeline renderPipeline = RenderPipeline(g_vertex_buffer_data, shader);

	while( glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window.GetWindow()) == 0 ){

		window.ClearWindow();

		renderPipeline.SetVBO();
		renderPipeline.Draw();
		
		window.SwapBufferWindow();
		glfwPollEvents();

	}

	renderPipeline.ClearVBO();

	glfwTerminate();

	return 0;
}

