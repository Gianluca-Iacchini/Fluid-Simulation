#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include "Transform.h"

namespace FluidSimulation {
	// Forward declaration
	class Scene;

	class Node
	{
	public:
		Transform* transform;
		Scene* const scene;

		std::vector<Component*> components;

		Node(glm::vec3 position, glm::fquat rotation, glm::vec3 scale, Node* parent = nullptr);
		Node(Transform* transform);
		Node(Node* parent = nullptr);

		void AddComponent(Component* component);
		static Node* GetRootNode() { return rootNode; }
		static void InitRootNode();
		void Update();


	private:
		static Node* rootNode;
	};
}

