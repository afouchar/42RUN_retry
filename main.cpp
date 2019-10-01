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

	string objFile = "models/spaceships/AK5/AK5.obj";
	if (argc > 1)
		objFile = argv[1];

	string objFile_A = "models/spaceships/DF6/DF6.obj";
	if (argc > 2)
		objFile_A = argv[2];

	Object *object = new Object(shader, objFile.c_str());
	Object *object_A = new Object(shader, objFile_A.c_str());
	
	renderPipeline->GenVAO(object);
	renderPipeline->SetMVP(camera, object); // not useful
	renderPipeline->GenBuffers(object);

	renderPipeline->GenVAO(object_A);
	renderPipeline->SetMVP(camera, object_A); // not useful
	renderPipeline->GenBuffers(object_A);

	camera->LookAt(camera->transform.position - vec3(0, 0, 0), vec3_up);

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
		renderPipeline->SetMVP(camera, object);
		renderPipeline->BindBuffers(object);
		renderPipeline->Draw(object);

		renderPipeline->UseProgram(object_A);
		renderPipeline->SetMVP(camera, object_A);
		renderPipeline->BindBuffers(object_A);
		renderPipeline->Draw(object_A);


		window.SwapBuffer();
		input.PollEvents();
	}

	renderPipeline->ClearBuffers(object);
	renderPipeline->ClearBuffers(object_A);
	window.Terminate();
	return 0;
}

