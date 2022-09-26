#pragma once
#include <glm/glm.hpp>

#include "KeyCodes.h"
#include "MouseCodes.h"

/* 
	Input management for non events from Hazel
	https://github.com/TheCherno/Hazel	
*/

namespace FluidSimulation {

	class Input
	{
	public:
		static bool GetKey(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}