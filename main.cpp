#include <stdio.h>
#include <stdlib.h>

#include "Window.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "RenderPipeline.hpp"
#include "Object.hpp"
#include "Input.hpp"
#include "Light.hpp"
#include "PathGenerator.hpp"


int main(int argc, char **argv) {

	Window window = Window(argv[0], vec2(1024, 768));
	Input input = Input(window.GetWindow());
	RenderPipeline *renderPipeline = new RenderPipeline();
	Shader *shader = new Shader("Shaders/Standard.vert", "Shaders/Standard.frag");
	Camera *camera = new Camera(70.0f, window.GetRatio(), 0.1f, 100.0f);
	Light *light = new Light(vec3(4, 4, 4));

	string objFile = "models/spaceships/AK5/AK5.obj";
	if (argc > 1)
		objFile = argv[1];

	Object *object = new Object(shader, objFile.c_str());
	PathGenerator pathGenerator = PathGenerator(shader, 3, 8.0f);
	
	renderPipeline->GenVAO(object);
	renderPipeline->GenBuffers(object);

	for (int i = 0 ; i < pathGenerator.chunks.size(); i++){
		renderPipeline->GenVAO(&pathGenerator.chunks[i]);
		renderPipeline->GenBuffers(&pathGenerator.chunks[i]);
	}

	camera->transform.position = vec3(0, 0, 15);
	camera->LookAt(object->transform.position, vec3_up);
	light->intensity = 15.0f;

	input.speed = 18.0f;

	while(!input.GetKeyPressed(GLFW_KEY_ESCAPE) && !window.IsClosed()){

		window.Clear();

		float deltaTime = input.DeltaTime();

		if (input.GetKeyPressed(GLFW_KEY_W)){
			object->transform.Translate(vec3_up * input.speed * deltaTime);
		}
		if (input.GetKeyPressed(GLFW_KEY_S)){
			object->transform.Translate(vec3_down * input.speed * deltaTime);
		}
		if (input.GetKeyPressed(GLFW_KEY_A)){
			object->transform.Translate(vec3_left * input.speed * deltaTime);
		}
		if (input.GetKeyPressed(GLFW_KEY_D)){
			object->transform.Translate(vec3_right * input.speed * deltaTime);
		}

		if (input.GetKeyPressed(GLFW_KEY_UP)){
			pathGenerator.speed += 10.0f * deltaTime;
			// std::cout << "(+) tube speed : " << pathGenerator.speed << std::endl;
		}
		if (input.GetKeyPressed(GLFW_KEY_DOWN)){
			pathGenerator.speed -= 10.0f * deltaTime;
			// std::cout << "(-) tube speed : " << pathGenerator.speed << std::endl;
		}

		for (int i = 0; i < pathGenerator.chunks.size(); i++){
			pathGenerator.chunks[i].transform.Translate(vec3_back * pathGenerator.speed * deltaTime);
		}

		camera->LookAt(object->transform.position, vec3_up);

		renderPipeline->UseProgram(object);
		renderPipeline->BindBuffers(object, camera, light);
		renderPipeline->Draw(object);

		for (int i = 0; i < pathGenerator.chunks.size(); i++){
			renderPipeline->UseProgram(&pathGenerator.chunks[i]);
			renderPipeline->BindBuffers(&pathGenerator.chunks[i], camera, light);
			renderPipeline->Draw(&pathGenerator.chunks[i]);
		}

	    if (pathGenerator.chunks.begin()->transform.position.z > camera->transform.position.z + pathGenerator.GetChunkLength()){
			pathGenerator.SwapFirstToLast(renderPipeline);
		}

		window.SwapBuffer();
		input.PollEvents();
	}

	renderPipeline->ClearBuffers(object);
	
	for (int i = 0; i < pathGenerator.chunks.size(); i++){
		renderPipeline->ClearBuffers(&pathGenerator.chunks[i]);
	}

	input.Terminate();
	window.Terminate();
	return 0;
}

