#pragma once

#include <glad/glad.h>
#include "Node/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace FluidSimulation {

	class Node;

	const float YAW = -90.f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	class Camera : public Component
	{
	public:

		enum Camera_Movement {
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			UP,
			DOWN
		};

		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;

		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;
		static Camera* mainCamera;


		Camera(Node* node, glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW, float pitch = PITCH);
		Camera(Node* node, float upX, float upY, float upZ, float yaw, float pitch);
		glm::mat4 GetViewMatrix();

		//void ProcessKeyboard(Camera_Movement direction, float deltaTime);
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
		void ProcessMouseScroll(float yoffset);

		void HideMouseCursor(bool isHidden);

		glm::mat4 CreateModelMatrix();

		void Update();

		void OnEvent(Event& e);

		bool OnMouseMove(MouseMovedEvent& e);


	private:
		void updateCameraVectors();
		float lastX;
		float lastY;
		float firstMouse = true;
	};
}