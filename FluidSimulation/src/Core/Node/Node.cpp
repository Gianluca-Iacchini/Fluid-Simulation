#include "fspch.h"
#include "Node.h"
#include "Core/Node/Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace FluidSimulation {

	Node* Node::rootNode = nullptr;

	//Add node at position position, with rotation rotation and scale scale, and a parent *parent
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

	//Add a node with parent *parent
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


	//Add a node with transform *transform
	Node::Node(Transform* transform) : scene(Scene::currentScene)
	{
		this->transform = transform;

		scene->AddNode(this);
	}

	//Create the root node
	void Node::InitRootNode()
	{
		rootNode = new Node();
	}

	//Add component *comp to this node
	void Node::AddComponent(Component* comp)
	{

		if (comp->node != this)
		{
			this->components.push_back(comp);
			comp->AddNode(this);
		}
	}

	//Update function is called every frame
	void Node::Update()
	{
		for (Component* comp : this->components)
		{
			if (comp->updateOnFrame)
				comp->Update();
		}

	}

}