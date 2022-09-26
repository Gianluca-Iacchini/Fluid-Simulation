#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <Core/Render/Shader.h>

/* Simple class for drawing text */
namespace FluidSimulation {

	struct FontCharacter
	{
		float advanceX;
		float advanceY;

		float bitmapWidth;
		float bitmapHeight;

		float bitmapLeft;
		float bitmapTop;

		float uvOffsetX;
		float uvOffsetY;
	};

	struct FontTextureAtlas
	{
		GLuint textureID;

		int width;
		int height;

		int lineSpacing;

		FontCharacter characters[128];

		FontTextureAtlas(FT_Face face, int h);

		~FontTextureAtlas()
		{
			glDeleteTextures(1, &textureID);
		}
	};

	class Text
	{
	public:

		Text(std::string text, float xPosition, float yPosition, unsigned int height = 48);
		void SetText(std::string text);
		void SetColor(glm::vec4 color);
		void Render();

	private:
		void setUpVAO();
		
		void InitFreeType();

		static std::unordered_map<int, FontTextureAtlas*> heightAtlasMap;



		float screenWidth = 0.f;
		float screenHeight = 0.f;

		float xPosition = 0.f;
		float yPosition = 0.f;

		int textHeight = 48;

		std::string text;
		glm::vec4 textColor = glm::vec4(1.f);

		FT_Face face;
		FT_Library ft;

		unsigned int textVAO;
		unsigned int textVBO;

		unsigned int quadVAO;
		unsigned int quadVBO;
		unsigned int quadEBO;

		FontTextureAtlas* atlas2;
		

		Shader drawTextProgram;
	};
}