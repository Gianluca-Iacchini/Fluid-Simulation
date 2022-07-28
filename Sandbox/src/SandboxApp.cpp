#include <FluidSimulation.h>
#include "Core/Events/MouseEvent.h"
#include "Core/Render/Cubemap.h"
#include "Core/Render/Text.h"

using namespace FluidSimulation;



class ExampleLayer : public FluidSimulation::Layer
{
public:



	ExampleLayer(Application* app) : Layer("Example")
	{
		

		initialScene = new Scene(app);
		Scene::currentScene = initialScene;
		Node::InitRootNode();

		Node* root = Node::GetRootNode();

		Shader* cubeShader = new Shader("1.model_loading.vert", "1.model_loading.frag");
		Model* redCube = new Cube();
		Model* blueCube = new Cube(glm::vec4(0, 0, 1, 1));
		Model* greenCube = new Cube(glm::vec4(0, 1, 0, 1));


		Node* cameraNode = new Node();

		Node* redNode = new Node();

		Render* render = new Render(redCube, cubeShader);



		cameraNode->transform->SetPosition(glm::vec3(0.f, 0.f, -2.0f));

		redNode->transform->SetPosition(glm::vec3(2.0f, 0.0f, -1.0f));

		redNode->AddComponent(render);


		// camera
		Camera* camera = new Camera(cameraNode);
		Camera::mainCamera = camera;
		camera->HideMouseCursor(true);

		fluid = Fluid(glm::vec3(128));
		fluid.Initialize();

		std::vector<std::string> cubeMapFaces{
			"..\\FluidSimulation\\res\\Cubemaps\\right.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\left.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\top.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\bottom.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\front.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\back.jpg"
		};

		cubeMap = new Cubemap(cubeMapFaces);

		text = new Text("FPS: 0\nFRAME TIME: 0ms", -0.95, 0.87, 32);
		text->SetColor(glm::vec4(1, 1, 0, 1));
	}

	void OnUpdate() override
	{
		

		Time time = initialScene->time;

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		fluid.DrawDepthBuffer();

		glDisable(GL_DEPTH_TEST);
		cubeMap->Render();
		glEnable(GL_DEPTH_TEST);

		initialScene->Update();
		
		float deltaTime = time.getDeltaTime();


		fluid.Update(deltaTime);

		float currentTime = initialScene->time.getCurrentTime();
		fps++;

		if (currentTime - lastTime >= 1.0f)
		{
			std::string num_text = std::to_string(1000.f / fps);
			std::string rounded = num_text.substr(0, num_text.find(".") + 4);
			text->SetText("FPS: " + std::to_string(fps) + "\nFRAME TIME: " + rounded + std::string("ms"));

			fps = 0;
			lastTime = currentTime;
		}

		text->Render();
	}

	void OnEvent(FluidSimulation::Event& event) override
	{
		Camera::mainCamera->OnEvent(event);
	}

private:
	Scene* initialScene = nullptr;
	Fluid fluid;
	Cubemap* cubeMap;
	Text* text;
	float lastTime = 0.f;
	unsigned int fps = 0;
};

class Sandbox : public FluidSimulation::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer(this));
	}

	~Sandbox()
	{

	}
};

FluidSimulation::Application* FluidSimulation::CreateApplication()
{
	return new Sandbox();
}