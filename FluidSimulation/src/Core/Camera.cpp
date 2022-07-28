#include "fspch.h"
#include "Camera.h"
#include "Node/Scene.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include "Core.h"

namespace FluidSimulation {

	Camera* Camera::mainCamera = nullptr;

	Camera::Camera(Node* node, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.f, 0.f, -1.f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		node->AddComponent(this);
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera::Camera(Node* node, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.f, 0.f, -1.f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		node->AddComponent(this);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}


	void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
	{
		xoffset *= MouseSensitivity * this->node->scene->time.getDeltaTime();
		yoffset *= MouseSensitivity * this->node->scene->time.getDeltaTime();


		glm::fquat qPitch = glm::angleAxis(-yoffset, glm::vec3(1, 0, 0));
		glm::fquat qYaw = glm::angleAxis(xoffset, glm::vec3(0, 1, 0));

		this->node->transform->SetRotation(qPitch * this->node->transform->GetRotation() * qYaw);
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		glm::mat4 rotation = glm::mat4_cast(this->node->transform->GetRotation());
		glm::mat4 translation = glm::mat4(1.0f);

		translation = glm::translate(translation, this->node->transform->GetPosition());

		return rotation * translation;

		//glm::vec3 euler = glm::eulerAngles(this->node->transform.rotation);
		//return glm::lookAt(this->node->transform.position, this->node->transform.position + Front, Up);
	}

	glm::mat4 Camera::CreateModelMatrix()
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(modelMatrix, this->node->transform->GetScale());
		modelMatrix = glm::translate(modelMatrix, this->node->transform->GetPosition());
		modelMatrix *= glm::mat4_cast(this->node->transform->GetRotation());

		return modelMatrix;
	}

	void Camera::Update()
	{
		glm::mat4 modelMatrix = CreateModelMatrix();
		glm::vec4 localZ = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) * modelMatrix;
		glm::vec3 z = glm::vec3(localZ);

		float deltaTime = this->node->scene->time.getDeltaTime();
		float moveSpeed = MovementSpeed * deltaTime;

		glm::vec3 position = this->node->transform->GetPosition();

		if (Input::IsKeyPressed(Key::W))
		{
			this->node->transform->SetPosition(position + this->node->transform->GetForwardAxis() * moveSpeed);
		}
		if (Input::IsKeyPressed(Key::S))
		{
			glm::vec3 position = this->node->transform->GetPosition();
			this->node->transform->SetPosition(position - this->node->transform->GetForwardAxis() * moveSpeed);
		}
		if (Input::IsKeyPressed(Key::D))
		{
			glm::vec3 position = this->node->transform->GetPosition();
			this->node->transform->SetPosition(position - this->node->transform->GetRightAxis() * moveSpeed);
		}
		if (Input::IsKeyPressed(Key::A))
		{
			glm::vec3 position = this->node->transform->GetPosition();
			this->node->transform->SetPosition(position + this->node->transform->GetRightAxis() * moveSpeed);
		}
		if (Input::IsKeyPressed(Key::E))
		{
			glm::vec3 position = this->node->transform->GetPosition();
			this->node->transform->SetPosition(position - glm::vec3(0, 1, 0) * moveSpeed);
		}
		if (Input::IsKeyPressed(Key::Q))
		{
			glm::vec3 position = this->node->transform->GetPosition();
			this->node->transform->SetPosition(position + glm::vec3(0, 1, 0) * moveSpeed);
		}

	}

	bool Camera::OnMouseMove(MouseMovedEvent& e)
	{
		
		float mouseX = e.GetX();
		float mouseY = e.GetY();

		if (firstMouse)
		{
			lastX = mouseX;
			lastY = mouseY;
			firstMouse = false;
		}



		float xOffset = mouseX - lastX;
		float yOffset = lastY - mouseY;
		lastX = mouseX;
		lastY = mouseY;
		this->ProcessMouseMovement(xOffset, yOffset);

		return false;
	}

	void Camera::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void Camera::updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void Camera::HideMouseCursor(bool isHidden)
	{
		GLFWwindow* window = (GLFWwindow*)this->node->scene->application->GetWindow().GetNativeWindow();
		if (isHidden)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Camera::OnMouseMove));

	}

}