#pragma once
#include "Model.h"
#include "Core/Node/Component.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Quick implementation of a Render component */
namespace FluidSimulation {

	class Render : public Component
	{
	public:
		Model* model;
		Shader* shader;

		Render(Model* model, Shader* shader);
		virtual void Update() override { Draw(); }

	private:
		void Draw();
		glm::mat4 CreateModelMatrix();
	};
}