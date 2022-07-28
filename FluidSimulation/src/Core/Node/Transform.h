#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

namespace FluidSimulation {

	class Transform
	{
	public:



		Transform(glm::vec3 position, glm::fquat rotation, glm::vec3 scale = glm::vec3(1.0f));
		Transform(Transform* parent, glm::vec3 position, glm::fquat rotation, glm::vec3 scale = glm::vec3(1.0f));
		Transform(glm::vec3 position);
		Transform(Transform* parent);
		Transform();


		glm::vec3 GetPosition();
		void SetPosition(glm::vec3 position);
		glm::fquat GetRotation();
		void SetRotation(glm::fquat rotation);
		void SetRotation(glm::vec3 rotation);
		glm::vec3 GetScale();
		void SetScale(glm::vec3 scale);

		glm::vec3 GetLocalPosition();
		void SetLocalPosition(glm::vec3 position);
		glm::fquat GetLocalRotation();
		void SetLocalRotation(glm::fquat rotation);
		glm::vec3 GetLocalScale();
		void SetLocalScale(glm::vec3 scale);

		glm::vec3 GetForwardAxis();
		glm::vec3 GetUpAxis();
		glm::vec3 GetRightAxis();

		glm::mat4 GetModelMatrix();
		void SetParent(Transform* parent);


	private:

		Transform* parent = nullptr;
		std::vector<Transform*> children;

		glm::vec3 position;
		glm::fquat rotation;
		glm::vec3 scale;

		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;

		void init(glm::vec3 position, glm::fquat rotation, glm::vec3 scale);

	};

}