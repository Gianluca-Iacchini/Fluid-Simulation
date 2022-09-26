#include "fspch.h"
#include "Text.h"
#include <Glad/glad.h>
#include <Core/Application.h>

using namespace FluidSimulation;



std::unordered_map<int, FontTextureAtlas*> Text::heightAtlasMap = std::unordered_map<int, FontTextureAtlas*>();

// Creates a Text text at screen position xPosition and yPosition with a font height of height
Text::Text(std::string text, float xPosition, float yPosition, unsigned int height)
{
	this->text = text;
	
	this->textHeight = height;

	this->xPosition = xPosition;
	this->yPosition = yPosition;

	this->screenWidth = Application::Get().GetWindow().GetWidth();
	this->screenHeight = Application::Get().GetWindow().GetHeight();

	setUpVAO();
	InitFreeType();

	drawTextProgram = Shader("Text/drawText.vert", "Text/drawText.frag");
}

// Helper function to manage text VAO
void Text::setUpVAO()
{
	glGenVertexArrays(1, &textVAO);
	glBindVertexArray(textVAO);

	glGenBuffers(1, &textVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
}

// Sets the text string to text
void FluidSimulation::Text::SetText(std::string text)
{

	this->text = text;
	
}

// Sets the text color to color
void FluidSimulation::Text::SetColor(glm::vec4 color)
{
	this->textColor = color;
}

// Initialization of freetype library
void FluidSimulation::Text::InitFreeType()
{
	if (FT_Init_FreeType(&ft))
	{
		FS_CORE_WARN("ERROR: Could not init FreeType Library.");
	}

	if (FT_New_Face(ft, "../FluidSimulation/res/fonts/arial.ttf", 0, &face))
	{
		FS_CORE_WARN("ERROR: Failed to load font.");
	}


	if (heightAtlasMap.find(this->textHeight) == heightAtlasMap.end())
	{
		heightAtlasMap.insert(std::pair<int, FontTextureAtlas*>(this->textHeight, new FontTextureAtlas(face, this->textHeight)));
	}

	atlas2 = new FontTextureAtlas(face, this->textHeight);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

// Called once every frame
void Text::Render()
{

	float sx = 2.0f / screenWidth;
	float sy = 2.0f / screenHeight;



	float x = xPosition;
	float y = yPosition;

	FontTextureAtlas* atlas = heightAtlasMap.at(this->textHeight);


	std::vector<glm::vec4> coords;

	// Computes new character position
	for (const char* p = this->text.c_str(); *p; p++)
	{
		// Newline
		if (*p == '\n')
		{
			y -= atlas->height * sy;
			x = xPosition;
			continue;
		}

		float x2 = x + atlas->characters[*p].bitmapLeft * sx;
		float y2 = -y - atlas->characters[*p].bitmapTop * sy;
		float w = atlas->characters[*p].bitmapWidth * sx;
		float h = atlas->characters[*p].bitmapHeight * sy;
	
	
		x += atlas->characters[*p].advanceX * sx;
		y += atlas->characters[*p].advanceY * sy;

		if (!w || !h)
			continue;

		coords.push_back(glm::vec4(x2, -y2, atlas->characters[*p].uvOffsetX, 0 ));
		coords.push_back(glm::vec4(x2 + w, -y2, atlas->characters[*p].uvOffsetX + atlas->characters[*p].bitmapWidth / atlas->width, 0));
		coords.push_back(glm::vec4(x2, -y2 - h, atlas->characters[*p].uvOffsetX , atlas->characters[*p].bitmapHeight / atlas->height)); //remember: each glyph occupies a different amount of vertical space
		coords.push_back(glm::vec4(x2 + w, -y2, atlas->characters[*p].uvOffsetX + atlas->characters[*p].bitmapWidth / atlas->width, 0));
		coords.push_back(glm::vec4(x2, -y2 - h, atlas->characters[*p].uvOffsetX, atlas->characters[*p].bitmapHeight / atlas->height));
		coords.push_back(glm::vec4(x2 + w, -y2 - h, atlas->characters[*p].uvOffsetX + atlas->characters[*p].bitmapWidth / atlas->width, atlas->characters[*p].bitmapHeight / atlas->height));
	}

	// Draw the text
	glViewport(0, 0, screenWidth, screenHeight);
	glBindVertexArray(this->textVAO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawTextProgram.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas->textureID);
	drawTextProgram.SetInt("textTexture", 0);
	drawTextProgram.SetVec4("textColor", this->textColor);

	glBindBuffer(GL_ARRAY_BUFFER, this->textVBO);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(glm::vec4), coords.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, coords.size());
}


// Texture atlas generation. All text characters are written to this texture.
FontTextureAtlas::FontTextureAtlas(FT_Face face, int h)
{
	{
		FT_Set_Pixel_Sizes(face, 0, h);
		FT_GlyphSlot glyphSlot = face->glyph;

		int rowWidth = 0;
		int rowHeight = 0;
		width = 0;
		height = 0;

		memset(characters, 0, sizeof(FontCharacter));

		for (int i = 32; i < 128; i++)
		{
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			{
				FS_CORE_WARN("TEXT: Failed to load character {:#c}", i);
				continue;
			}

			rowWidth += glyphSlot->bitmap.width;
			rowHeight = std::fmax(rowHeight, glyphSlot->bitmap.rows);
		}

		this->width = rowWidth;
		this->height = rowHeight;
		this->lineSpacing = face->glyph->metrics.height;

		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, rowWidth, rowHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);



		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ox = 0;

		for (int i = 32; i < 128; i++)
		{
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			{
				FS_CORE_WARN("Failed to load character {:#c}", i);
				continue;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, 0, glyphSlot->bitmap.width, glyphSlot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyphSlot->bitmap.buffer);





			characters[i].advanceX = glyphSlot->advance.x >> 6;
			characters[i].advanceY = glyphSlot->advance.y >> 6;

			characters[i].bitmapWidth = glyphSlot->bitmap.width;
			characters[i].bitmapHeight = glyphSlot->bitmap.rows;

			characters[i].bitmapLeft = glyphSlot->bitmap_left;
			characters[i].bitmapTop = glyphSlot->bitmap_top;

			characters[i].uvOffsetX = (float)ox / (float) this->width;

			ox += glyphSlot->bitmap.width;
		}
	}


}
