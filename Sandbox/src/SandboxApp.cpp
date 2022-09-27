#include <FluidSimulation.h>
#include "Core/Events/MouseEvent.h"
#include "Core/Render/Cubemap.h"

using namespace FluidSimulation;



class ExampleLayer : public FluidSimulation::Layer
{
public:



	ExampleLayer(Application* app) : Layer("Example")
	{
		
		// Initialize camera and cubes around the scene
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
		Node* blueNode = new Node();
		Node* greenNode = new Node();

		Render* renderRed = new Render(redCube, cubeShader);
		Render* renderBlue = new Render(blueCube, cubeShader);
		Render* renderGreen = new Render(greenCube, cubeShader);



		cameraNode->transform->SetPosition(glm::vec3(0.f, 0.f, -2.0f));

		redNode->transform->SetPosition(glm::vec3(2.0f, 0.0f, -1.0f));
		blueNode->transform->SetPosition(glm::vec3(-2.0f, 0.0f, -1.0f));
		greenNode->transform->SetPosition(glm::vec3(0.0f, 0.0f, -2.0f));

		redNode->AddComponent(renderRed);
		blueNode->AddComponent(renderBlue);
		greenNode->AddComponent(renderGreen);


		Camera* camera = new Camera(cameraNode);
		Camera::mainCamera = camera;
		camera->HideMouseCursor(true);

		// Initialize fluid
		fluid = Fluid(glm::vec3(128));
		fluid.Initialize();

		// Initialize cubemap
		std::vector<std::string> cubeMapFaces{
			"..\\FluidSimulation\\res\\Cubemaps\\right.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\left.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\top.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\bottom.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\front.jpg",
			"..\\FluidSimulation\\res\\Cubemaps\\back.jpg"
		};

		//cubeMap = new Cubemap(cubeMapFaces);

	}

	// Called once every frame
	void OnUpdate() override
	{
		

		Time time = initialScene->time;

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Write scene to fluid depthbuffer texture
		fluid.DrawDepthBuffer();

		// Render cubemap
		//glDisable(GL_DEPTH_TEST);
		//cubeMap->Render();
		//glEnable(GL_DEPTH_TEST);

		// Update scene
		initialScene->Update();
		
		// Update time
		float deltaTime = time.getDeltaTime();

		// Update and render fluid.
		fluid.Update(deltaTime);

		// Update fps text value


	}

	void OnEvent(FluidSimulation::Event& event) override
	{
		Camera::mainCamera->OnEvent(event);
	}

private:
	Scene* initialScene = nullptr;
	Fluid fluid;
	Cubemap* cubeMap;
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