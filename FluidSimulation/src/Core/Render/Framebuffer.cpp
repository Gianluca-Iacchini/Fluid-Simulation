#include "fspch.h"
#include "Framebuffer.h"

void CheckError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		FS_CORE_WARN("ERROR::GENERIC::%x\n", err);
	}
}

namespace FluidSimulation {

	Framebuffer::Framebuffer(GLsizei width, GLsizei height, GLsizei depth, int nComponents)
	{
		this->initFramebuffer3D(width, height, depth, nComponents);

	}
	Framebuffer::Framebuffer(glm::vec3 size, int nComponents)
	{
		this->initFramebuffer3D(size.x, size.y, size.z, nComponents);
	}

	Framebuffer::Framebuffer(GLsizei width, GLsizei height, int nComponents)
	{
		this->initFramebuffer2D(width, height, nComponents);
	}
	Framebuffer::Framebuffer(glm::vec2 size, int nComponents)
	{
		this->initFramebuffer2D(size.x, size.y, nComponents);
	}

	// Initialize a framebuffer with a color texture of given width, height, and depth
	void Framebuffer::initFramebuffer3D(GLsizei width, GLsizei height, GLsizei depth, int nComponents)
	{
		GLuint fboHandle;
		glGenFramebuffers(1, &fboHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_3D, textureHandle);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		switch (nComponents) {
		case 1:
			glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, width, height, depth, 0, GL_RED, GL_FLOAT, 0);
			break;
		case 2:
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RG32F, width, height, depth, 0, GL_RG, GL_FLOAT, 0);
			break;
		case 3:
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F, width, height, depth, 0, GL_RGB, GL_FLOAT, 0);
			break;
		case 4:
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, width, height, depth, 0, GL_RGBA, GL_FLOAT, 0);
			break;
		}

		GLuint colorbuffer;
		glGenRenderbuffers(1, &colorbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_3D, 0);

		this->FBOHandle = fboHandle;
		this->colorTextureHandle = textureHandle;
		this->framebufferDim = GL_TEXTURE_3D;

		this->width = width;
		this->height = height;
		this->depth = depth;
		this->nComponents = nComponents;
	}

	// Same as previous function but for 2D textures
	void Framebuffer::initFramebuffer2D(GLsizei width, GLsizei height, int nComponents)
	{
		GLuint fboHandle;
		glGenFramebuffers(1, &fboHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		switch (nComponents) {
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);
			break;
		case 2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT, 0);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
			break;
		}

		GLuint colorbuffer;
		glGenRenderbuffers(1, &colorbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			FS_CORE_WARN("FRAMEBUFFER 2D TEXTURE ERROR");

		this->FBOHandle = fboHandle;
		this->colorTextureHandle = textureHandle;
		this->framebufferDim = GL_TEXTURE_2D;

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		this->width = width;
		this->height = height;
		this->depth = 1;
		this->nComponents = nComponents;
	}

	// Swap PingPongFramebuffer buffers;
	void Framebuffer::Swap(PingPongFramebuffer& ppFBO)
	{
		Framebuffer* temp = ppFBO.readFBO;
		ppFBO.readFBO = ppFBO.writeFBO;
		ppFBO.writeFBO = temp;
	}

	Framebuffer::PingPongFramebuffer Framebuffer::CreateDoubleFramebuffer(Framebuffer fbo)
	{
		GLsizei width = fbo.width;
		GLsizei height = fbo.height;
		
		GLenum fboDim = fbo.framebufferDim;
		int nComponents = fbo.nComponents;

		Framebuffer* readFBO;
		Framebuffer* writeFBO;

		if (fboDim == GL_TEXTURE_3D)
		{
			GLsizei depth = fbo.depth;
			readFBO = new Framebuffer(width, height, depth, nComponents);
			writeFBO = new Framebuffer(width, height, depth, nComponents);
		}
		else 
		{
			readFBO = new Framebuffer(width, height, nComponents);
			writeFBO = new Framebuffer(width, height, nComponents);
		}


		PingPongFramebuffer ppfbo = { readFBO, writeFBO };
		return ppfbo;
	}

	Framebuffer::PingPongFramebuffer Framebuffer::CreateDoubleFramebuffer(glm::vec3 size, int nComponents)
	{
		PingPongFramebuffer ppfbo = { new Framebuffer(size, nComponents), new Framebuffer(size, nComponents) };
		return ppfbo;
	}

	Framebuffer::PingPongFramebuffer Framebuffer::CreateDoubleFramebuffer(glm::vec2 size, int nComponents)
	{
		PingPongFramebuffer ppfbo = { new Framebuffer(size, nComponents), new Framebuffer(size, nComponents) };
		return ppfbo;
	}
	
	// Attach depth texture to an existing framebuffer
	void Framebuffer::AttachDepthBufferTexture()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

		glGenTextures(1, &depthTextureHandle);
		glBindTexture(GL_TEXTURE_2D, depthTextureHandle);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureHandle, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			FS_CORE_WARN("FRAMEBUFFER DEPTH ERROR");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}