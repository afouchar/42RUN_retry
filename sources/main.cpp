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
#include "Text.hpp"
#include "GameBehaviour.hpp"

#include "PathGenerator.hpp"


int main(int argc, char **argv) {

	Window window = Window(argv[0], vec2(1024, 768));
	Input input = Input((*window.GetWindow()));
	Shader *shader = new Shader("Shaders/Standard.vert", "Shaders/Standard.frag");
	Camera *camera = new Camera(70.0f, window.GetRatio(), 0.1f, 1000.0f);
	Light *light = new Light(vec3(4, 4, 4));

	// Shader *textShader = new Shader("Shaders/StandardText.vert", "Shaders/StandardText.frag");
	// Text *textFPS = new Text(textShader, &window, "Resources/Holstein.png");

	string objFile = "Models/spaceships/AK5/AK5.obj";
	if (argc > 1)
		objFile = argv[1];

	Object *object = new Object((*shader), objFile.c_str());
	PathGenerator pathGenerator = PathGenerator((*shader), 6, 8.0f);

	camera->transform.position = vec3(0, 0, 15);
	// camera->transform.position = vec3(35, 45, 55);
	camera->LookAt(object->transform.position, vec3_up);
	light->intensity = 15.0f;

	input.speed = 18.0f;
	// textFPS->fontSize = 32;

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
			object->transform.Rotate(vec3_forward, input.speed * deltaTime);
			// object->transform.RotateAround(vec3_zero);
		}
		if (input.GetKeyPressed(GLFW_KEY_D)){
			object->transform.Rotate(vec3_forward, -input.speed * deltaTime);
			// object->transform.RotateAround(vec3_zero);
		}
		if (input.GetKeyPressed(GLFW_KEY_Q)){
			object->transform.Scale(vec3_one * -(deltaTime / 100.0f));
		}
		if (input.GetKeyPressed(GLFW_KEY_E)){
			object->transform.Scale(vec3_one * (deltaTime / 100.0f));
		}

		if (input.GetKeyPressed(GLFW_KEY_UP)){
			pathGenerator.speed += 10.0f * deltaTime;
			// std::cout << "(+) tube speed : " << pathGenerator.speed << std::endl;
		}
		if (input.GetKeyPressed(GLFW_KEY_DOWN)){
			pathGenerator.speed -= 10.0f * deltaTime;
			// std::cout << "(-) tube speed : " << pathGenerator.speed << std::endl;
		}

		pathGenerator.MovePath((*object), deltaTime);

		camera->LookAt(object->transform.position, vec3_up);

		RenderPipeline::Draw();

	    // if ((*pathGenerator.chunks.begin())->transform.position.z >= pathGenerator.GetHalfChunkLength()){
		// 	std::cout << "SWAP" << std::endl;
		// 	pathGenerator.SwapFirstToLast();
		// }


		// float fps = (1.0 / deltaTime);
		// ostringstream buff;
		// buff << fps;

		// textFPS->Draw(string("FPS : " + buff.str()).c_str());

		window.SwapBuffer();
		input.PollEvents();
		GameBehaviour::UpdateCollisions();
	}

	std::cout << std::endl << "==========================" << std::endl << "END PROGRAM" << std::endl;

	RenderPipeline::Clear();
	// delete object;
	// delete light;
	// delete camera;
	// delete shader;

	// textFPS->Clear();

	input.Terminate();
	window.Terminate();
	return 0;
}

