#include <stdio.h>
#include <stdlib.h>

#include "Window.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "RenderPipeline.hpp"
#include "Object.hpp"
#include "Input.hpp"


int main(int argc, char **argv) {

	Window window = Window(argv[0], vec2(1024, 768));
	Input input = Input(window.GetWindow());

	RenderPipeline *renderPipeline = new RenderPipeline();

	Shader *shader = new Shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.frag");

	Camera *camera = new Camera(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	camera->transform.position = vec3(4, 3, -3);
	camera->LookAt(vec3_zero);

	Object *object = new Object(shader);
	
	renderPipeline->SetMVP(camera, object);
	renderPipeline->GenBuffers(object);

	while(!input.GetKeyPressed(GLFW_KEY_ESCAPE) && !window.IsClosed()){

		window.Clear();

		renderPipeline->UseProgram(object);

		//GET INPUT AND SET MVP//
//_____________________________________________//
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Compute new orientation
	camera->transform.UpdateDirection(input.GetMouseDirection());
	input.ResetMousePosition();

	// Move forward
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_UP ) == GLFW_PRESS){
		camera->transform.position += camera->transform.GetDirection() * deltaTime * input.speed;
	}
	// Move backward
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_DOWN ) == GLFW_PRESS){
		camera->transform.position -= camera->transform.GetDirection() * deltaTime * input.speed;
	}
	// Strafe right
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_RIGHT ) == GLFW_PRESS){
		camera->transform.position += camera->transform.GetRight() * deltaTime * input.speed;
	}
	// Strafe left
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT ) == GLFW_PRESS){
		camera->transform.position -= camera->transform.GetRight() * deltaTime * input.speed;
	}

//_____________________________________________//

		camera->LookAt(camera->transform.position + camera->transform.GetDirection(), camera->transform.GetUp());
		// camera->LookAt(vec3_zero, camera->transform.GetUp());
		renderPipeline->SetMVP(camera, object);

//_____________________________________________//

	lastTime = currentTime;
//_____________________________________________//

		renderPipeline->BindBuffers(object);

		renderPipeline->Draw(object);

		window.SwapBuffer();
		glfwPollEvents();
	}

	renderPipeline->ClearBuffers(object);
	window.Terminate();
	return 0;
}

