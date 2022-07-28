#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <btBulletDynamicsCommon.h>


class conv
{
public:
	static btVector3 glm3ToBtVec3(glm::vec3 vec)
	{
		return btVector3(vec.x, vec.y, vec.z);
	}
	static glm::vec3 btVec3ToGlm3(btVector3 vec)
	{
		return glm::vec3(vec.x(), vec.y(), vec.z());
	}
	static btQuaternion fquatToBtQuaternion(glm::fquat quat)
	{
		return btQuaternion(quat.x, quat.y, quat.z, quat.w);
	}
	static glm::fquat btQuaternionToFquat(btQuaternion quat)
	{
		return glm::fquat(quat.w(), quat.x(), quat.y(), quat.z());
	}
};