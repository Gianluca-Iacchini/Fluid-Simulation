#include "fspch.h"
#include "FluidStep.h"

namespace FluidSimulation {

    FluidStep::FluidStep(glm::vec3 gridScale, int nComponents)
    {

        this->gridScale = gridScale;
        readBuffer = InitFramebuffer3D(gridScale, nComponents);
        writeBuffer = InitFramebuffer3D(gridScale, nComponents);
    }
    
	FluidStep::FluidStep(glm::vec2 screenSize, int nComponents)
	{
		this->gridScale.x = screenSize.x;
		this->gridScale.y = screenSize.y;
		this->gridScale.z = -1;

		readBuffer = InitFramebuffer2D(screenSize, nComponents);
		writeBuffer = InitFramebuffer2D(screenSize, nComponents);
	}

    FluidStep::FluidStep() {
        gridScale = glm::vec3(1);
        
        readBuffer.fboHandle = 0;
		readBuffer.textureHandle = 0;

        writeBuffer.fboHandle = 0;
		writeBuffer.textureHandle = 0;
    }

    FluidStep::FBO FluidStep::InitFramebuffer3D(glm::vec3 gridScale, int nComponents)
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
                glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, gridScale.x, gridScale.y, gridScale.z, 0, GL_RED, GL_HALF_FLOAT, NULL);
                break;
            case 2:
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RG16F, gridScale.x, gridScale.y, gridScale.z, 0, GL_RG, GL_HALF_FLOAT, NULL);
                break;
            case 3:
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F, gridScale.x, gridScale.y, gridScale.z, 0, GL_RGB, GL_HALF_FLOAT, NULL);
                break;
            default:
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, gridScale.x, gridScale.y, gridScale.z, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
                break;
        }

        GLuint colorBuffer;
        glGenRenderbuffers(1, &colorBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);

        FBO fbo;
        fbo.fboHandle = fboHandle;
        fbo.textureHandle = textureHandle;

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        return fbo;
    }

	FluidStep::FBO FluidStep::InitFramebuffer2D(glm::vec2 gridScale, int nComponents)
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
			glTexImage2D(GL_TEXTURE_3D, 0, GL_R16F, gridScale.x, gridScale.y, 0, GL_RED, GL_HALF_FLOAT, NULL);
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_3D, 0, GL_RG16F, gridScale.x, gridScale.y, 0, GL_RG, GL_HALF_FLOAT, NULL);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_3D, 0, GL_RGB16F, gridScale.x, gridScale.y, 0, GL_RGB, GL_HALF_FLOAT, NULL);
            break;
        default:
            glTexImage2D(GL_TEXTURE_3D, 0, GL_RGBA16F, gridScale.x, gridScale.y, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
			break;
		}

		GLuint colorBuffer;
		glGenRenderbuffers(1, &colorBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);

		FBO fbo;
		fbo.fboHandle = fboHandle;
		fbo.textureHandle = textureHandle;

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		return fbo;
	}

    unsigned int FluidStep::Read(int textureNum)
    {

        return 0;
    }

    void FluidStep::Write(unsigned int quadVAO)
    {
        //glViewport(0, 0, gridScale.x, gridScale.y);


        //glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        //glBindVertexArray(quadVAO);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glBindVertexArray(0);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //target.Swap(&target.readBuffer, &target.writeBuffer);
        //glViewport(0, 0, screenSize.x, screenSize.y);
        //textureNum = 0;
    }

    void FluidStep::Swap()
    {
        //unsigned int temp = textureRead;
        //textureRead = textureWrite;
        //textureWrite = temp;
        FBO temp = readBuffer;
        readBuffer = writeBuffer;
        writeBuffer = temp;
    }


}