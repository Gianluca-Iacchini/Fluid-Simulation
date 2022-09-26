#pragma once

/* Basic class for time management */
namespace FluidSimulation {

	class Time {
	public:
		Time();
		float maxSecPerFrame = 1.f / 60.f;
		void Update();
		double getCurrentTime() const;
		double getDeltaTime() const;


	private:
		double deltaTime;
		double lastTime;
	};
}