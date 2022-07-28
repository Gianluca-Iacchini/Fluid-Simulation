#include "fspch.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace FluidSimulation {

	using namespace glm;

	Transform::Transform()
	{
		this->init(vec3(0.0f), fquat(1, 0, 0, 0), vec3(1.0f));
	}

	Transform::Transform(Transform* parent)
	{
		this->init(vec3(0.0f), fquat(1, 0, 0, 0), vec3(1.0f));
		this->SetParent(parent);

	}

	Transform::Transform(vec3 position, fquat rotation, vec3 scale)
	{
		this->init(position, rotation, scale);
	}

	Transform::Transform(Transform* parent, vec3 position, fquat rotation, vec3 scale)
	{
		this->init(position, rotation, scale);
		this->SetParent(parent);
	}

	Transform::Transform(vec3 position)
	{
		this->init(position, fquat(1, 0, 0, 0), vec3(1.0f));
	}

	void Transform::init(vec3 position, fquat rotation, vec3 scale)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	void Transform::SetParent(Transform* parent)
	{
		this->parent = parent;

		if (parent != nullptr)
			parent->children.push_back(this);
	}


	void Transform::SetPosition(vec3 position)
	{
		for (Transform* child : children)
			child->SetPosition(position + child->GetLocalPosition());

		this->position = position;
	}

	vec3 Transform::GetPosition()
	{
		return this->position;
	}

	void Transform::SetRotation(fquat rotation)
	{
		for (Transform* child : children)
			child->SetRotation(rotation * child->GetLocalRotation());

		this->rotation = rotation;
	}

	void Transform::SetRotation(vec3 rotation)
	{
		float roll = radians(rotation.x);
		float yaw = radians(rotation.y);
		float pitch = radians(rotation.z);

		this->SetRotation(fquat(vec3(roll, yaw, pitch)));
	}

	fquat Transform::GetRotation()
	{
		return this->rotation;
	}

	void Transform::SetScale(vec3 scale)
	{
		for (Transform* child : children)
			child->SetScale(scale * child->GetLocalScale());

		this->scale = scale;
	}

	vec3 Transform::GetScale()
	{
		return this->scale;
	}

	void Transform::SetLocalPosition(vec3 position)
	{
		vec3 newPosition = parent == nullptr ? position : parent->position + position;
		this->SetPosition(newPosition);
	}

	vec3 Transform::GetLocalPosition()
	{
		vec3 localPosition = parent == nullptr ? this->position : this->position - parent->position;
		return localPosition;
	}

	void Transform::SetLocalRotation(fquat rotation)
	{
		fquat newRotation = parent == nullptr ? rotation : parent->rotation * rotation;
		this->SetRotation(newRotation);
	}

	fquat Transform::GetLocalRotation()
	{
		glm::fquat localRotation = parent == nullptr ? this->rotation : conjugate(parent->rotation) * this->rotation;

		return localRotation;
	}

	void Transform::SetLocalScale(vec3 scale)
	{
		vec3 newScale = parent == nullptr ? scale : parent->scale * scale;
		this->SetScale(newScale);
	}

	vec3 Transform::GetLocalScale()
	{
		vec3 localScale = parent == nullptr ? this->scale : this->scale / parent->scale;
		return localScale;
	}

	vec3 Transform::GetForwardAxis()
	{
		fquat localRotation = this->GetLocalRotation();
		return vec3(0, 0, 1) * localRotation;
	}

	vec3 Transform::GetRightAxis()
	{
		fquat localRotation = this->GetLocalRotation();
		return vec3(1, 0, 0) * localRotation;
	}

	vec3 Transform::GetUpAxis()
	{
		fquat localRotation = this->GetLocalRotation();
		return vec3(0, 1, 0) * localRotation;
	}

	mat4 Transform::GetModelMatrix()
	{
		mat4 modelMatrix = mat4(1.f);

		modelMatrix = translate(modelMatrix, this->position);
		modelMatrix *= mat4_cast(this->rotation);
		modelMatrix = glm::scale(modelMatrix, this->scale);

		return modelMatrix;
	}
}