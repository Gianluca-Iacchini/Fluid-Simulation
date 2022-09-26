#pragma once
/* 
	Mesh Function from learnopengl.com
	https://learnopengl.com/Model-Loading/Mesh	
*/

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include "Shader.h"

using namespace std;

namespace FluidSimulation {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct Texture
	{
		unsigned int id;
		string type;
		string path;
	};

	class Mesh {
	public:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		glm::vec4 color;

		unsigned int quadVAO;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			this->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			setupMesh();
		}

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, glm::vec4 color)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->color = color;

			setupMesh();

		}

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, glm::vec4 color)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			this->color = color;

			setupMesh();

		}

		void Draw(Shader& shader)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				string number;
				string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++);
				else if (name == "texture_height")
					number = std::to_string(heightNr++);

				shader.SetInt((name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}


			shader.SetBool("drawTexture", !textures.empty());

			shader.SetVec4("variantColor", this->color);
			glBindVertexArray(quadVAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);
		}
	private:
		unsigned int screenVBO, screenEBO;

		void setupMesh()
		{
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &screenVBO);
			glGenBuffers(1, &screenEBO);

			glBindVertexArray(quadVAO);

			glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

			glBindVertexArray(0);
		}

	};
}