#include "fspch.h"
#include "Cubemap.h"
#include <stb_image.h>
#include "Core/Camera.h"
#include "Core/Application.h"

using namespace FluidSimulation;

Cubemap::Cubemap(vector<string> faces)
{
	this->faces = faces;

	float screenWidth = Application::Get().GetWindow().GetWidth();
	float screenHeight = Application::Get().GetWindow().GetHeight();

	this->screenSize = glm::vec2(screenWidth, screenHeight);

	initTextures();
	initVertices();

	cubeMapShader = new Shader("Cubemap/cube_map.vert", "Cubemap/cube_map.frag");
}

void Cubemap::Render()
{
	Camera* camera = Camera::mainCamera;

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera->Zoom), screenSize.x / screenSize.y, 0.001f, 100.0f);

	viewMatrix = glm::mat4(glm::mat3(viewMatrix));
	glm::mat4 viewProjection = projectionMatrix * viewMatrix;

	glBindVertexArray(cubeMapVAO);
	glViewport(0, 0, screenSize.x, screenSize.y);

	cubeMapShader->Use();
	cubeMapShader->SetMat4("viewProjection",viewProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	cubeMapShader->SetInt("cubeMap", 0);


	glDrawArrays(GL_TRIANGLES, 0, 36);
}

Cubemap::Cubemap()
{
	float screenWidth = Application::Get().GetWindow().GetWidth();
	float screenHeight = Application::Get().GetWindow().GetHeight();

	this->screenSize = glm::vec2(screenWidth, screenHeight);

	initTextures();
	initVertices();

	cubeMapShader = new Shader("Cubemap/cube_map.vert", "Cubemap/cube_map.frag");
}

void FluidSimulation::Cubemap::initTextures()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;

	unsigned char* data;

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			FS_CORE_WARN("ERROR TO LOAD CUBEMAP AT PATH: {0}", faces[i]);
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void FluidSimulation::Cubemap::initVertices()
{
	glGenVertexArrays(1, &cubeMapVAO);
	glGenBuffers(1, &cubeMapVBO);
	glBindVertexArray(cubeMapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}
