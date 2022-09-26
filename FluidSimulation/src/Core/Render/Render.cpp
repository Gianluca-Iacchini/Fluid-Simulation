#include "fspch.h"
#include "Render.h"
#include "Core/Camera.h"
#include "Core/Node/Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace FluidSimulation {

	// Initialize Render component with a model *model and a shader *shader
	Render::Render(Model* model, Shader* shader)
	{
		this->model = model;
		this->shader = shader;
	}

	// Draw is called once every frame.
	void Render::Draw()
	{

		if (!this->node)
			return;
		float screenWidth = this->node->scene->application->m_Window->GetWidth();
		float screenHeight = this->node->scene->application->m_Window->GetHeight();

		Camera* camera = Camera::mainCamera;

		// Create mvp matrix
		glm::mat4 viewMatrix = camera->GetViewMatrix();
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
		glm::mat4 modelMatrix = this->CreateModelMatrix();

		glViewport(0, 0, screenWidth, screenHeight);

		// Draw the model model using the shader shader
		this->shader->Use();
		shader->SetMat4("view", viewMatrix);
		shader->SetMat4("projection", projectionMatrix);
		shader->SetMat4("model", modelMatrix);
		model->Draw(*shader);
	}

	// Returns this node mvp matrix
	glm::mat4 Render::CreateModelMatrix()
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		modelMatrix = glm::translate(modelMatrix, this->node->transform->GetPosition());
		modelMatrix *= glm::mat4_cast(this->node->transform->GetRotation());
		modelMatrix = glm::scale(modelMatrix, this->node->transform->GetScale());

		return modelMatrix;
	}
}