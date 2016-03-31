#pragma once
#include "include.h"
#include "Colour.h"
#include "Font.h"

#define GetGlyphs font->getGlyphs(fontSize)

class Text
{
public:
	Text();
	~Text();

	void setFont(Font* pFont);

	void setString(std::string pString);

	std::string getString() { return string; }

	void setColour(Colour pColour);
	
	void setPosition(vec2f pPos);
	vec2f getPosition() { return pos; }

	void setScale(vec2f pScale);

	void setCharSize(unsigned char pSize);

	unsigned char getCharSize() { return fontSize; }

	void draw();

	void operator=(const Text& rhs)
	{
		font = rhs.font;
		fontSize = rhs.fontSize;
		string = rhs.string;
		colour = rhs.colour;
		pos = rhs.pos;
		scale = rhs.scale;
		loadedGlyphSize = rhs.loadedGlyphSize;
		modelUniform = rhs.modelUniform;
	}

	GLint posAttrib;
	GLint colAttrib;
	GLint texAttrib;

	frect getLocalBounds() { return localBounds; }
	frect getGlobalBounds() { return frect(localBounds.left + pos.x, localBounds.bot + pos.y, localBounds.width, localBounds.height); }

private:

	void updateBounds()
	{
		if ((font == nullptr) || (string.size() == 0) || (scale.x == 0) || (scale.y == 0) || (fontSize == 0))
		{
			return;
		}

		const char *p = string.c_str();

		float minY = FLT_MAX, maxY = FLT_MIN;
		float minX = FLT_MAX, maxX = FLT_MIN;

		vec2f rgp(0.f, 0.f);

		while (*p != '\0')
		{
			if (*p == '\n')
			{
				rgp.x = pos.x;
				rgp.y -= fontSize;
				++p;
			}
			else
			{
				font->bindGlyph(fontSize, *p);

				float fX = (rgp.x * scale.x) + (GetGlyphs->getPositions(*p).x * scale.x);
				float fY = (rgp.y * scale.y) + (GetGlyphs->getPositions(*p).y * scale.y);
				float w = GetGlyphs->getPixelSize(*p).x * scale.x;
				float h = GetGlyphs->getPixelSize(*p).y * scale.y;

				if (fX < minX) minX = fX;
				if (fX + w > maxX) maxX = fX + w;
				if (fY > maxY) maxY = fY;
				if (fY - h < minY) minY = fY - h;

				rgp.x += ((font->getGlyphs(fontSize)->getAdvance(*p)).x >> 6);
				rgp.y += ((font->getGlyphs(fontSize)->getAdvance(*p)).y >> 6);

				++p;
			}
		}

		localBounds.left = minX; localBounds.bot = minY;
		localBounds.width = maxX - minX; localBounds.height = maxY - minY;
	}

	frect localBounds;

	Shader* shader;
	Font* font;
	//GlyphContainer* glyphs;

	std::string string;
	Colour colour;
	vec2f pos;
	vec2f scale;
	unsigned char fontSize;
	unsigned char loadedGlyphSize;
	bool needUpdate;

	GLuint modelUniform;

	GLuint vbo;
	GLuint vao;
};