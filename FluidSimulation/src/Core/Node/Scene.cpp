#include "fspch.h"
#include "Scene.h"


namespace FluidSimulation {
	Scene* Scene::currentScene = nullptr;

	// Create a scene for Application *app
	Scene::Scene(Application* app) : application(app)
	{
		physics = new Physics(time);
	}

	// Update is called every frame
	void Scene::Update()
	{
		time.Update();
		physics->Update();
		for (Node* node : sceneNodes)
		{
			node->Update();
		}
	}

	// Add a node to the scene.
	void Scene::AddNode(Node* node)
	{
		this->sceneNodes.push_back(node);
	}
}

