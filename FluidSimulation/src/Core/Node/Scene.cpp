#include "fspch.h"
#include "Scene.h"


namespace FluidSimulation {
	Scene* Scene::currentScene = nullptr;

	Scene::Scene(Application* app) : application(app)
	{
		physics = new Physics(time);
	}

	void Scene::Update()
	{
		time.Update();
		physics->Update();
		for (Node* node : sceneNodes)
		{
			node->Update();
		}
	}


	void Scene::AddNode(Node* node)
	{
		this->sceneNodes.push_back(node);
	}
}

