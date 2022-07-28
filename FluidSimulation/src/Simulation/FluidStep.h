#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Core/Render/Shader.h"


namespace FluidSimulation {

	class FluidStep
	{
	private:
		typedef struct FBO
		{
			unsigned int fboHandle;
			unsigned int textureHandle;
		} FBO;

	public:

		FluidStep(glm::vec3 gridScale, int nComponents);
		FluidStep(glm::vec2 gridScale, int nComponents);
		FluidStep();


		unsigned int Read(int textureNum = -1);
		void Write(unsigned int quadVAO);
		void Swap();
		static void SetArrays(unsigned int quadVAO, unsigned int screenVBO, unsigned int screenEBO);
		glm::vec2 GetFlatTextureSize();

		glm::vec3 gridScale;
		FBO readBuffer;
		FBO writeBuffer;


	private:

		FBO InitFramebuffer3D(glm::vec3 gridScale, int nComponents);
		FBO InitFramebuffer2D(glm::vec2 gridScale, int nComponents);
	};

}