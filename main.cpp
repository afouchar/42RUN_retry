#include <stdio.h>
#include <stdlib.h>

#include "Window.hpp"
#include "Shader.hpp"
#include "RenderPipeline.hpp"


int main(int argc, char **argv) {

	Window window = Window(argv[0], vec2(1024, 768));

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window.GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	//Shader shader = Shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.frag");
	//RenderPipeline renderPipeline = RenderPipeline(g_vertex_buffer_data, shader);

	RenderPipeline renderPipeline = RenderPipeline(g_vertex_buffer_data);
	Shader shader = Shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.frag");
	renderPipeline.SetShader(shader);

	while( glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window.GetWindow()) == 0 ){

		window.ClearWindow();

		renderPipeline.UseProgram();

		renderPipeline.SetVBO();
		renderPipeline.Draw();

		window.SwapBufferWindow();
		glfwPollEvents();

	}

	renderPipeline.ClearVBO();

	glfwTerminate();

	return 0;
}

