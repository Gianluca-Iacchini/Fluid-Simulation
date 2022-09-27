#pragma once
#include "Node.h"
#include <vector>
#include "Core/Time.H"
#include "Core/Application.h"

/* Naive implementation of a scene class for scene management */
namespace FluidSimulation
{
	class Scene
	{
	public:
		Application* application;

		static Scene* currentScene;
		Scene(Application* app);
		void Update();
		void AddNode(Node* node);

		Time& const time = Time();

	private:
		std::vector<Node*> sceneNodes;
	};
}



