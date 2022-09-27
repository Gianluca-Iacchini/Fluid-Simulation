#pragma once
/*
	Model class from learnopengl.com
	https://learnopengl.com/Model-Loading/Model
*/


#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>



#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>



using namespace std;

namespace FluidSimulation {

	class Model
	{
	public:
		vector<Texture> textures_loaded;
		vector<Mesh> meshes;
		string directory;
		bool gammaCorrection;

		Model() : gammaCorrection(false)
		{

		}
		virtual void Draw(Shader& shader)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				meshes[i].Draw(shader);
			}
		}
	};


}