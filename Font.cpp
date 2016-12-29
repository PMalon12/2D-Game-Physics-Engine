#pragma once
#include "Font.h"
#include "Game.h"

#define FTLib Game::fTManager.fTLib

GlyphContainer::GlyphContainer() : charSize(0), loaded(false) {}

GlyphContainer::~GlyphContainer() {}

const GLuint* GlyphContainer::getGlyphs()
{
	return glyphs;
}

GLuint GlyphContainer::getGlyph(unsigned char pAscSymbol)
{
	return glyphs[pAscSymbol - 32];
}

vec2i GlyphContainer::getPixelSize(unsigned char pAscSymbol)
{
	return sizes[pAscSymbol - 32];
}

vec2i GlyphContainer::getPositions(unsigned char pAscSymbol)
{
	return positions[pAscSymbol - 32];
}

vec2l GlyphContainer::getAdvance(unsigned char pAscSymbol)
{
	return advances[pAscSymbol - 32];
}

void GlyphContainer::bindGlyph(unsigned char pAscSymbol)
{
	glBindTexture(GL_TEXTURE_2D, glyphs[pAscSymbol - 32]);
}

void GlyphContainer::release()
{
	if (!loaded) { return; }
	glActiveTexture(GL_TEXTURE0);
	glDeleteTextures(NO_PRINTABLE_CHARS, glyphs);
	std::cout << "GLYPHS RELEASED: " << int(charSize) << std::endl;
}

void GlyphContainer::load(unsigned char pCharSize, std::string pFontLocation, FT_Face pFace)
{
	charSize = pCharSize;
	loaded = true;

	if (pFace == nullptr)
	{
		assert(false);
	}

	glActiveTexture(GL_TEXTURE0);


	FT_Set_Pixel_Sizes(pFace, 0, charSize);
	//FT_Set_Pixel_Sizes(pFace, 0, 500);

	for (char i = 0; i < NO_PRINTABLE_CHARS; ++i)
	{
		char ascSymbol = i + (unsigned char)(32);

		glGenTextures(1, &glyphs[i]);
		glBindTexture(GL_TEXTURE_2D, glyphs[i]);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		FT_Error err = FT_Load_Char(pFace, ascSymbol, FT_LOAD_RENDER);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, pFace->glyph->bitmap.width, pFace->glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pFace->glyph->bitmap.buffer);

		sizes[i].x = pFace->glyph->bitmap.width;
		sizes[i].y = pFace->glyph->bitmap.rows;

		positions[i].x = pFace->glyph->bitmap_left;
		positions[i].y = pFace->glyph->bitmap_top;

		advances[i].x = pFace->glyph->advance.x;
		advances[i].y = pFace->glyph->advance.y;
	}
}


Font::Font()
{
	noRefFont = 0;
	for (auto i = 0; i < MAX_NO_CHAR_SIZES; ++i)
	{
		glyphContainer[i] = nullptr;
		noRefs[i] = -1;
	}
}

Font::~Font()
{
	releaseAllGlyphs();
}

void Font::loadFromFile(std::string pFontLocation)
{
	if (face != nullptr)
	{
		FT_Done_Face(face);
	}

	if (FT_New_Face(FTLib, pFontLocation.c_str(), 0, &face)) 
	{
		fprintf(stderr, "Could not open font %s\n", pFontLocation);
	}
	else
	{
		std::cout << "Font \"" << face->family_name << " " << face->style_name << "\" loaded from file: \"" << pFontLocation << "\"" << std::endl;
	}

	fontLocation = pFontLocation;
}

void Font::loadGlyphs(unsigned char pSize)
{
	if (noRefs[pSize] == -1) { ++noRefs[pSize]; }
	noRefs[pSize]++;
	if (pSize == 32)
	{
		std::cout << "LOADING 32: " << int(noRefs[32]) << std::endl;
	}
	usedFontSizes.insert(pSize);
	if (glyphContainer[pSize] != nullptr)
	{
		return;
	}

	glyphContainer[pSize] = new GlyphContainer();
	glyphContainer[pSize]->load(pSize, fontLocation, face);
}

void Font::releaseGlyphs(unsigned char pSize)
{
	if(noRefs[pSize] > 0)
		noRefs[pSize]--;
	if (pSize == 32)
	{
		std::cout << "RELEASING 32: " << int(noRefs[32]) << std::endl;
	}
	if (noRefs[pSize] == 0)
	{
		glyphContainer[pSize]->release();
		delete glyphContainer[pSize];
		glyphContainer[pSize] = nullptr;
	}
}

void Font::cleanUpGlyphs()
{
	for (auto itr = usedFontSizes.begin(); itr != usedFontSizes.end(); ++itr)
	{
		if (noRefs[(*itr)] == 0)
		{
			if (glyphContainer[*itr] != nullptr)
			{
				glyphContainer[*itr]->release();
				delete glyphContainer[*itr];
				glyphContainer[*itr] = nullptr;
			}
			itr = usedFontSizes.erase(itr);
		}
	}
}

void Font::bindGlyph(unsigned char pCharSize, unsigned char pAscSymbol)
{
	glyphContainer[pCharSize]->bindGlyph(pAscSymbol);
}

GlyphContainer* Font::getGlyphs(unsigned char pSize)
{
	if (glyphContainer[pSize] == nullptr)
	{
		assert(false);
	}
	return glyphContainer[pSize];
}

void Font::releaseAllGlyphs()
{
	for (auto i = 0; i < MAX_NO_CHAR_SIZES; ++i)
	{
		if (glyphContainer[i] != nullptr)
		{
			glyphContainer[i]->release();
			delete glyphContainer[i];
			glyphContainer[i] = nullptr;
		}
		noRefs[i] = 0;
	}
}