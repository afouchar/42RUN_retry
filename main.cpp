#include <stdio.h>
#include <stdlib.h>

#include "Window.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "RenderPipeline.hpp"
#include "Object.hpp"


int main(int argc, char **argv) {

	Window window = Window(argv[0], vec2(1024, 768));

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window.GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	RenderPipeline *renderPipeline = new RenderPipeline();

	Shader *shader = new Shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.frag");

	Camera *camera = new Camera(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	camera->transform.position = vec3(4, 3, -3);
	camera->LookAt(vec3_zero);

	// RenderPipeline renderPipeline = RenderPipeline(g_vertex_buffer_data);
	// Shader shader = Shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.frag");
	// renderPipeline.SetShader(shader);

	// renderPipeline.SetVBO();

	Object *object = new Object(shader);
	
	renderPipeline->SetMVP(camera, object);
	renderPipeline->GenBuffers(object);

	while( glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window.GetWindow()) == 0 ){

		window.ClearWindow();

		renderPipeline->UseProgram(object);

		// renderPipeline->SetMVP(camera, object);

		renderPipeline->BindBuffers(object);

		renderPipeline->Draw(object);

		window.SwapBufferWindow();
		glfwPollEvents();
	}

	renderPipeline->ClearBuffers(object);

	glfwTerminate();

	return 0;
}

