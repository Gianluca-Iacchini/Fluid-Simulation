#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>


namespace FluidSimulation {
	/* Framebuffer wrapper class */
	class Framebuffer
	{
	public:

		typedef struct PingPongFramebuffer
		{
			Framebuffer* readFBO;
			Framebuffer* writeFBO;

		} PingPongFramebuffer;

		Framebuffer(GLsizei width, GLsizei height, GLsizei depth, int nComponents);
		Framebuffer(glm::vec3 size, int nComponents);

		Framebuffer(GLsizei width, GLsizei height, int nComponents);
		Framebuffer(glm::vec2 size, int nComponents);

		static void Swap(PingPongFramebuffer& ppFBO);
		
		static PingPongFramebuffer CreateDoubleFramebuffer(Framebuffer fbo);
		static PingPongFramebuffer CreateDoubleFramebuffer(glm::vec3 size, int nComponents);
		static PingPongFramebuffer CreateDoubleFramebuffer(glm::vec2 size, int nComponents);

		inline GLuint GetFramebufferHandle() { return FBOHandle; }
		inline GLuint GetColorTextureHandle() { return colorTextureHandle; }
		inline GLuint GetDepthTextureHandle() { return depthTextureHandle; }
		inline GLenum GetFramebufferDimension() { return framebufferDim; }

		inline glm::vec3 GetFramebufferSize() { return glm::vec3(width, height, depth); }

		void AttachDepthBufferTexture();

	private:
		GLuint FBOHandle;
		GLuint colorTextureHandle;
		GLuint depthTextureHandle;
		GLenum framebufferDim;

		GLsizei width;
		GLsizei height;
		GLsizei depth;
		
		int nComponents;

		void initFramebuffer3D(GLsizei width, GLsizei height, GLsizei depth, int nComponents);
		void initFramebuffer2D(GLsizei width, GLsizei height, int nComponents);


	};
}


