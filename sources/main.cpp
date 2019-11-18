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
#include "Ship.hpp"


int main(int argc, char **argv) {

	GameBehaviour::CreateContext(argv[0], vec2(1024, 768));

	Shader *shader = new Shader("Shaders/Standard.vert", "Shaders/Standard.frag");
	Camera *camera = new Camera(70.0f, GameBehaviour::window->GetRatio(), 0.1f, 1000.0f);
	Light *light = new Light(vec3(4, 4, 4));

	// Shader *textShader = new Shader("Shaders/StandardText.vert", "Shaders/StandardText.frag");
	// Text *textFPS = new Text(textShader, &window, "Resources/Holstein.png");

	string objFile = "Models/spaceships/AK5/AK5.obj";
	if (argc > 1)
		objFile = argv[1];

	Ship *ship = new Ship((*shader), objFile.c_str());
	ship->SetTag("Player");
	ship->transform.Translate(vec3(0, -19, 0));


	PathGenerator pathGenerator = PathGenerator((*shader), 16, 8.0f);
	// PathGenerator pathGenerator = PathGenerator((*shader), 6, 8.0f);

	camera->transform.position = vec3(0, 0, 30);
	// camera->transform.position = vec3(50, 50, 30);
	camera->LookAt(ship->transform.position, vec3_up);
	light->intensity = 15.0f;

	// textFPS->fontSize = 32;

	while(!GameBehaviour::input->GetKeyPressed(GLFW_KEY_ESCAPE) && !GameBehaviour::window->IsClosed()){

		GameBehaviour::BeginFrame();
		GameBehaviour::Clock();
		GameBehaviour::UpdateObjectScripts();

		if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_UP)){
			pathGenerator.speed += 10.0f * GameBehaviour::DeltaTime();
			// std::cout << "(+) tube speed : " << pathGenerator.speed << std::endl;
		}
		if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_DOWN)){
			pathGenerator.speed -= 10.0f * GameBehaviour::DeltaTime();
			// std::cout << "(-) tube speed : " << pathGenerator.speed << std::endl;
		}
		camera->LookAt(ship->transform.position, vec3_up);

		RenderPipeline::Draw();

		// float fps = (1.0 / GameBehaviour::DeltaTime());
		// ostringstream buff;
		// buff << fps;

		// textFPS->Draw(string("FPS : " + buff.str()).c_str());

		GameBehaviour::EndFrame();
		GameBehaviour::UpdateCollisions();
	}

	std::cout << std::endl << "==========================" << std::endl << "END PROGRAM" << std::endl;

	RenderPipeline::Clear();
	// delete object;
	// delete light;
	// delete camera;
	// delete shader;

	// textFPS->Clear();
	GameBehaviour::Terminate();
	return 0;
}

