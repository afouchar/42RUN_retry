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


	PathGenerator pathGenerator = PathGenerator((*shader), 16, 20.0f);

	// camera->transform.position = vec3(50, 50, 10);
	camera->transform.position = vec3(0, 0, 15);
	camera->LookAt(ship->transform.position, vec3_up);
	camera->SetTarget(ship->transform);
	light->intensity = 15.0f;

	// textFPS->fontSize = 32;

	while(!GameBehaviour::input->GetKeyPressed(GLFW_KEY_ESCAPE) && !GameBehaviour::window->IsClosed()){

		GameBehaviour::BeginFrame();
		GameBehaviour::Clock();
		GameBehaviour::UpdateObjectScripts();
		GameBehaviour::LateUpdateObjectScripts();
		RenderPipeline::Draw();

		// float fps = (1.0 / GameBehaviour::DeltaTime());
		// if (fps < 200.0f)
		// 	std::cerr << "FPS : " << fps << std::endl;

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

