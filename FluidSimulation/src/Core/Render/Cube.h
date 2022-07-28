#pragma once
#include "Model.h"
#include <vector>

namespace FluidSimulation {


	class Cube : public Model
	{
	public:
		Cube(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
		{
			setUpCube(color);
		}

		void Draw(Shader& shader) override
		{
			Model::Draw(shader);
		}

	private:
		void setUpCube(glm::vec4 color)
		{
			vector<Vertex> vertices;
			Vertex vertex;
			vector<unsigned int> indices = { 0,1,2,0,2,3 };

			// Bottom face

			glm::vec3 position = glm::vec3(-0.5f, -0.5f, -0.5f);
			glm::vec2 textUV = glm::vec2(0.0f, 0.0f);
			glm::vec3 normal = glm::vec3(0.f, -1.0f, 0.f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, -0.5f, -0.5f);
			textUV = glm::vec2(1.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, -0.5f, 0.5f);
			textUV = glm::vec2(1.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(-0.5f, -0.5f, 0.5f);
			textUV = glm::vec2(0.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			Mesh mesh = Mesh(vertices, indices, color);
			meshes.push_back(mesh);
			vertices.clear();
			// Top Face
			normal = glm::vec3(0.0f, 1.0f, 0.0f);

			position = glm::vec3(-0.5f, 0.5f, 0.5f);
			textUV = glm::vec2(0.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, 0.5f, 0.5f);
			textUV = glm::vec2(1.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, 0.5f, -0.5f);
			textUV = glm::vec2(1.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(-0.5f, 0.5f, -0.5f);
			textUV = glm::vec2(0.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			mesh = Mesh(vertices, indices, color);
			meshes.push_back(mesh);
			vertices.clear();

			// Front face
			normal = glm::vec3(0.0f, 0.0f, 1.0f);

			position = glm::vec3(-0.5f, -0.5f, 0.5f);
			textUV = glm::vec2(0.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, -0.5f, 0.5f);
			textUV = glm::vec2(1.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, 0.5f, 0.5f);
			textUV = glm::vec2(1.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(-0.5f, 0.5f, 0.5f);
			textUV = glm::vec2(0.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			mesh = Mesh(vertices, indices, color);
			meshes.push_back(mesh);
			vertices.clear();

			//Back face
			normal = glm::vec3(0.0f, 0.0f, -1.0f);

			position = glm::vec3(-0.5f, -0.5f, -0.5f);
			textUV = glm::vec2(0.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(-0.5f, 0.5f, -0.5f);
			textUV = glm::vec2(1.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, 0.5f, -0.5f);
			textUV = glm::vec2(1.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, -0.5f, -0.5f);
			textUV = glm::vec2(0.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			mesh = Mesh(vertices, indices, color);
			meshes.push_back(mesh);
			vertices.clear();

			//Right face
			normal = glm::vec3(1.0f, 0.0f, 0.0f);

			position = glm::vec3(0.5f, -0.5f, 0.5f);
			textUV = glm::vec2(0.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, -0.5f, -0.5f);
			textUV = glm::vec2(1.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, 0.5f, -0.5f);
			textUV = glm::vec2(1.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(0.5f, 0.5f, 0.5f);
			textUV = glm::vec2(0.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			mesh = Mesh(vertices, indices, color);
			meshes.push_back(mesh);
			vertices.clear();

			//Left face
			normal = glm::vec3(-1.0f, 0.0f, 0.0f);

			position = glm::vec3(-0.5f, -0.5f, -0.5f);
			textUV = glm::vec2(0.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(-0.5f, -0.5f, 0.5f);
			textUV = glm::vec2(1.0f, 0.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(-0.5f, 0.5f, 0.5f);
			textUV = glm::vec2(1.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			position = glm::vec3(-0.5f, 0.5f, -0.5f);
			textUV = glm::vec2(0.0f, 1.0f);
			vertex = createVertex(position, textUV, normal);
			vertices.push_back(vertex);

			mesh = Mesh(vertices, indices, color);
			meshes.push_back(mesh);
			vertices.clear();

		}

		Vertex createVertex(glm::vec3 position, glm::vec2 textUV, glm::vec3 normal)
		{
			Vertex vertex;
			glm::vec3 emptyVector = glm::vec3(0.f, 0.f, 0.f);

			vertex.Position = position;
			vertex.Normal = normal;
			vertex.TexCoords = textUV;
			vertex.Tangent = emptyVector;
			vertex.Bitangent = emptyVector;

			return vertex;
		}

	};
}