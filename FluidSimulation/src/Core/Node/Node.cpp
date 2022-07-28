#include "fspch.h"
#include "Node.h"
#include "Core/Node/Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace FluidSimulation {

	Node* Node::rootNode = nullptr;

	Node::Node(glm::vec3 position, glm::fquat rotation, glm::vec3 scale, Node* parent) : scene(Scene::currentScene)
	{
		Transform* transform = nullptr;

		if (parent)
			transform = parent->transform;
		else if (rootNode)
			transform = rootNode->transform;

		this->transform = new Transform(transform, position, rotation, scale);

		scene->AddNode(this);
	}

	Node::Node(Node* parent) : scene(Scene::currentScene)
	{
		Transform* transform = nullptr;

		if (parent)
			transform = parent->transform;
		else if (rootNode)
			transform = rootNode->transform;

		this->transform = new Transform(transform, glm::vec3(0.f), glm::fquat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

		scene->AddNode(this);
	}



	Node::Node(Transform* transform) : scene(Scene::currentScene)
	{
		this->transform = transform;

		scene->AddNode(this);
	}


	void Node::InitRootNode()
	{
		rootNode = new Node();
	}


	void Node::AddComponent(Component* comp)
	{

		if (comp->node != this)
		{
			this->components.push_back(comp);
			comp->AddNode(this);
		}
	}

	void Node::Update()
	{
		for (Component* comp : this->components)
		{
			if (comp->updateOnFrame)
				comp->Update();
		}

	}

}