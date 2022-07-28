#include "fspch.h"
#include "Time.h"
#include <glfw/glfw3.h>

namespace FluidSimulation {

	void Time::Update()
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

	double Time::getCurrentTime() const
	{
		return glfwGetTime();
	}

	double Time::getDeltaTime() const
	{
		return this->deltaTime;
	}

	Time::Time()
	{
		this->deltaTime = 0.0;
		this->lastTime = 0.0;
	}
}