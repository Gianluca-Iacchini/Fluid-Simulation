#include "fspch.h"
#include "Framebuffer.h"



namespace FluidSimulation {

	Framebuffer::Framebuffer()
	{
		this->FBOHandle = -1;
		this->colorTextureHandle = -1;
		this->framebufferDim = -1;

		this->width = 0;
		this->height = 0;
		this->depth = 0;
		this->nComponents = 0;
	}

	// Initialize a 3D framebuffer with width width, height height and depth depth and nComponents components.
	Framebuffer::Framebuffer(GLsizei width, GLsizei height, GLsizei depth, int nComponents)
	{
		this->initFramebuffer3D(width, height, depth, nComponents);

	}

	// Initialize a 3D framebuffer with width, height and depth given by the size parameter and nComponents components
	Framebuffer::Framebuffer(glm::vec3 size, int nComponents)
	{
		this->initFramebuffer3D(size.x, size.y, size.z, nComponents);
	}

	// Initialize a 2D framebuffer with width width, height height and depth and nComponents components.
	Framebuffer::Framebuffer(GLsizei width, GLsizei height, int nComponents)
	{
		this->initFramebuffer2D(width, height, nComponents);
	}
	Framebuffer::Framebuffer(glm::vec2 size, int nComponents)
	{
		this->initFramebuffer2D(size.x, size.y, nComponents);
	}

	// Framebuffer 3D initialization helper function
	void Framebuffer::initFramebuffer3D(GLsizei width, GLsizei height, GLsizei depth, int nComponents)
	{
		// Framebuffer creation
		GLuint fboHandle;
		glGenFramebuffers(1, &fboHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

		// Framebuffer texture creation
		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_3D, textureHandle);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Switch on nComponents to determine the number of channels for the texture
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

		// Attaching texture to framebuffer colorbuffer
		GLuint colorbuffer;
		glGenRenderbuffers(1, &colorbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		// Setting up variables and clearing state
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

	// Framebuffer 2D initialization helper function
	void Framebuffer::initFramebuffer2D(GLsizei width, GLsizei height, int nComponents)
	{
		// Framebuffer creation
		GLuint fboHandle;
		glGenFramebuffers(1, &fboHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

		// Framebuffer texture creation
		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Switch on nComponents to determine the number of channels for the texture
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

		// Attaching texture to framebuffer colorbuffer
		GLuint colorbuffer;
		glGenRenderbuffers(1, &colorbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			FS_CORE_WARN("FRAMEBUFFER 2D TEXTURE ERROR");

		// Setting up variables and clearing state
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

	PingPongFramebuffer::PingPongFramebuffer()
	{
		this->readFBO = Framebuffer();
		this->writeFBO = Framebuffer();
	}

	// Create a PingPongFramebuffer by creating two framebuffers with fbo nComponents, width, height and depth (if 3D)
	PingPongFramebuffer::PingPongFramebuffer(Framebuffer fbo)
	{
		glm::vec3 fboSize = fbo.GetFramebufferSize();

		GLsizei width = fboSize.x;
		GLsizei height = fboSize.y;
		
		GLenum fboDim = fbo.GetFramebufferDimension();

		int nComponents = fbo.GetFramebufferNComponents();


		if (fboDim == GL_TEXTURE_3D)
		{
			GLsizei depth = fboSize.z;
			this->readFBO = Framebuffer(width, height, depth, nComponents);
			this->writeFBO = Framebuffer(width, height, depth, nComponents);

		}
		else 
		{
			this->readFBO = Framebuffer(width, height, nComponents);
			this->writeFBO = Framebuffer(width, height, nComponents);
		}
	}

	// Create a 3D PingPongFramebbufer by creating two framebuffers with size size and nComponents nComponents
	PingPongFramebuffer::PingPongFramebuffer(glm::vec3 size, int nComponents)
	{
		this->writeFBO = Framebuffer(size, nComponents);
		this->readFBO = Framebuffer(size, nComponents);
	}
	

	// Create a 2D PingPongFramebbufer by creating two framebuffers with size size and nComponents nComponents
	PingPongFramebuffer::PingPongFramebuffer(glm::vec2 size, int nComponents)
	{
		this->writeFBO = Framebuffer(size, nComponents);
		this->readFBO = Framebuffer(size, nComponents);
	}
	
	// Attach depth texture to an existing framebuffer
	void Framebuffer::AttachDepthBufferTexture()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

		// Generating depth texture
		glGenTextures(1, &depthTextureHandle);
		glBindTexture(GL_TEXTURE_2D, depthTextureHandle);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		// Attaching depth texture
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureHandle, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			FS_CORE_WARN("FRAMEBUFFER DEPTH ERROR");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}