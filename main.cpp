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

	// Shader *shader = new Shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.frag");
	Shader *shader = new Shader("Shaders/TexturedVertexShader.vert", "Shaders/TexturedFragmentShader.frag");

	Camera *camera = new Camera(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	camera->transform.position = vec3(4, 3, -3);
	camera->LookAt(vec3_zero); // not useful

	string objFile = "models/spaceships/AK5/AK5.obj";
	if (argc > 1)
		objFile = argv[1];

	Object *object = new Object(shader, objFile.c_str());
	
	renderPipeline->SetMVP(camera, object); // not useful
	renderPipeline->GenBuffers(object);
	// renderPipeline->GenBuffers(object_A);

	while(!input.GetKeyPressed(GLFW_KEY_ESCAPE) && !window.IsClosed()){

		window.Clear();

		renderPipeline->UseProgram(object);

		float deltaTime = input.DeltaTime();

		camera->transform.UpdateDirection(input.GetMouseDirection());
		input.ResetMousePosition();

		if (input.GetKeyPressed(GLFW_KEY_UP)){
			camera->transform.position += camera->transform.GetDirection() * deltaTime * input.speed;
		}
		if (input.GetKeyPressed(GLFW_KEY_DOWN)){
			camera->transform.position -= camera->transform.GetDirection() * deltaTime * input.speed;
		}
		if (input.GetKeyPressed(GLFW_KEY_RIGHT)){
			camera->transform.position += camera->transform.Right() * deltaTime * input.speed;
		}
		if (input.GetKeyPressed(GLFW_KEY_LEFT)){
			camera->transform.position -= camera->transform.Right() * deltaTime * input.speed;
		}

		camera->LookAt(camera->transform.position + camera->transform.GetDirection(), camera->transform.Up());
		// camera->LookAt(vec3_zero, camera->transform.Up());
		renderPipeline->SetMVP(camera, object);

		renderPipeline->BindBuffers(object);

		renderPipeline->Draw(object);

		// renderPipeline->UseProgram(object_A);
		// renderPipeline->SetMVP(camera, object_A);
		// renderPipeline->BindBuffers(object_A);
		// renderPipeline->Draw(object_A);


		window.SwapBuffer();
		input.PollEvents();
	}

	renderPipeline->ClearBuffers(object);
	window.Terminate();
	return 0;
}

