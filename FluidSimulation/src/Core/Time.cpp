#include "fspch.h"
#include "Time.h"
#include <glfw/glfw3.h>

namespace FluidSimulation {

	// Called every frame
	void Time::Update()
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

	// Wrapper function for glfwGetTime
	double Time::getCurrentTime() const
	{
		return glfwGetTime();
	}

	// Returns time between frames
	double Time::getDeltaTime() const
	{
		return this->deltaTime;
	}

	// Default constructor
	Time::Time()
	{
		this->deltaTime = 0.0;
		this->lastTime = 0.0;
	}
}