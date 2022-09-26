#include "fspch.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace FluidSimulation {

	using namespace glm;

	// Initialize transform with default values of position at origin, no rotation and no scale.
	Transform::Transform()
	{
		this->init(vec3(0.0f), fquat(1, 0, 0, 0), vec3(1.0f));
	}

	// Initialize transform with default values and add *parent as this transform parent.
	Transform::Transform(Transform* parent)
	{
		this->init(vec3(0.0f), fquat(1, 0, 0, 0), vec3(1.0f));
		this->SetParent(parent);

	}

	// Initialize transform at position position, with rotation rotation and scale scale
	Transform::Transform(vec3 position, fquat rotation, vec3 scale)
	{
		this->init(position, rotation, scale);
	}

	// Initialize transform with *parent parent, at position position with rotation rotation and scale scale.
	Transform::Transform(Transform* parent, vec3 position, fquat rotation, vec3 scale)
	{
		this->init(position, rotation, scale);
		this->SetParent(parent);
	}

	// Initialize transform at position position with default rotation and scale.
	Transform::Transform(vec3 position)
	{
		this->init(position, fquat(1, 0, 0, 0), vec3(1.0f));
	}

	// Initialization helper function
	void Transform::init(vec3 position, fquat rotation, vec3 scale)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	// Set transform parent as *parent
	void Transform::SetParent(Transform* parent)
	{
		this->parent = parent;

		if (parent != nullptr)
			parent->children.push_back(this);
	}

	// Set transform position at position
	void Transform::SetPosition(vec3 position)
	{
		for (Transform* child : children)
			child->SetPosition(position + child->GetLocalPosition());

		this->position = position;
	}

	// Return this transform position
	vec3 Transform::GetPosition()
	{
		return this->position;
	}

	// Set transform rotation at rotation
	void Transform::SetRotation(fquat rotation)
	{
		for (Transform* child : children)
			child->SetRotation(rotation * child->GetLocalRotation());

		this->rotation = rotation;
	}

	// Set transform rotation using roll, yaw and pitch (rotation.x, rotation.y and rotation.z respectively)
	void Transform::SetRotation(vec3 rotation)
	{
		float roll = radians(rotation.x);
		float yaw = radians(rotation.y);
		float pitch = radians(rotation.z);

		this->SetRotation(fquat(vec3(roll, yaw, pitch)));
	}

	// Return transform rotation
	fquat Transform::GetRotation()
	{
		return this->rotation;
	}

	// Set transform scale as scale
	void Transform::SetScale(vec3 scale)
	{
		for (Transform* child : children)
			child->SetScale(scale * child->GetLocalScale());

		this->scale = scale;
	}

	// Return transform scale
	vec3 Transform::GetScale()
	{
		return this->scale;
	}

	// Set transform local position (relative to parent) as position
	void Transform::SetLocalPosition(vec3 position)
	{
		vec3 newPosition = parent == nullptr ? position : parent->position + position;
		this->SetPosition(newPosition);
	}

	// Return transform local position
	vec3 Transform::GetLocalPosition()
	{
		vec3 localPosition = parent == nullptr ? this->position : this->position - parent->position;
		return localPosition;
	}

	// Set transform local rotation (relative to parent) as rotation
	void Transform::SetLocalRotation(fquat rotation)
	{
		fquat newRotation = parent == nullptr ? rotation : parent->rotation * rotation;
		this->SetRotation(newRotation);
	}

	// Returns local rotation
	fquat Transform::GetLocalRotation()
	{
		glm::fquat localRotation = parent == nullptr ? this->rotation : conjugate(parent->rotation) * this->rotation;

		return localRotation;
	}

	// Set transform local scale (relative to parent) as scale
	void Transform::SetLocalScale(vec3 scale)
	{
		vec3 newScale = parent == nullptr ? scale : parent->scale * scale;
		this->SetScale(newScale);
	}

	// Returns local scale
	vec3 Transform::GetLocalScale()
	{
		vec3 localScale = parent == nullptr ? this->scale : this->scale / parent->scale;
		return localScale;
	}

	// Returns transform forwards axis (z positive).
	vec3 Transform::GetForwardAxis()
	{
		fquat localRotation = this->GetLocalRotation();
		return vec3(0, 0, 1) * localRotation;
	}

	// Returns transform right axis (x positive)
	vec3 Transform::GetRightAxis()
	{
		fquat localRotation = this->GetLocalRotation();
		return vec3(1, 0, 0) * localRotation;
	}

	// Returns transform up axis (y positive)
	vec3 Transform::GetUpAxis()
	{
		fquat localRotation = this->GetLocalRotation();
		return vec3(0, 1, 0) * localRotation;
	}

	// Returns the model matrix of this transform
	mat4 Transform::GetModelMatrix()
	{
		mat4 modelMatrix = mat4(1.f);

		modelMatrix = translate(modelMatrix, this->position);
		modelMatrix *= mat4_cast(this->rotation);
		modelMatrix = glm::scale(modelMatrix, this->scale);

		return modelMatrix;
	}
}