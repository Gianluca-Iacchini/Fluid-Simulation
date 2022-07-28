#pragma once
#include "Node.h"
#include "Core/Physics/Physics.h"
#include <vector>
#include "Time.h"
#include "Core/Application.h"

namespace FluidSimulation
{
	class Scene
	{
	public:
		Physics* physics;
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



