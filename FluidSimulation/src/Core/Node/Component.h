#pragma once



namespace FluidSimulation {
	//Forward declaration
	class Node;

	class Component
	{
	public:
		Node* node = nullptr;
		bool updateOnFrame = true;

		Component() {}
		void AddNode(Node* node) { this->node = node; }
		virtual void Update() {};
	};
}