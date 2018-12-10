#pragma once
#include "Include.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define NO_PRINTABLE_CHARS 95
#define MAX_NO_CHAR_SIZES 256

class GlyphContainer
{
public:
	GlyphContainer();
	~GlyphContainer();

	const GLuint* getGlyphs();

	GLuint getGlyph(unsigned char pAscSymbol);

	vec2i getPixelSize(unsigned char pAscSymbol);

	vec2i getPositions(unsigned char pAscSymbol);

	vec2l getAdvance(unsigned char pAscSymbol);

	void bindGlyph(unsigned char pAscSymbol);

	void load(unsigned char pCharSize, std::string pFontLocation, FT_Face pFace);

	void release();

private:

	bool loaded;
	unsigned char charSize;
	GLuint glyphs[NO_PRINTABLE_CHARS];
	vec2i sizes[NO_PRINTABLE_CHARS];
	vec2i positions[NO_PRINTABLE_CHARS];
	vec2l advances[NO_PRINTABLE_CHARS];

};

class Font
{
public:
	Font();
	~Font();

	void loadFromFile(std::string pFontLocation);

	void loadGlyphs(unsigned char pSize);

	void releaseGlyphs(unsigned char pSize);

	void cleanUpGlyphs();

	void bindGlyph(unsigned char pCharSize, unsigned char pAscSymbol);

	GlyphContainer* getGlyphs(unsigned char pSize);

	void releaseAllGlyphs();

	uint32_t fontID;

	FT_Face face;
	GlyphContainer* glyphContainer[MAX_NO_CHAR_SIZES];
	short noRefs[MAX_NO_CHAR_SIZES];
	short noRefFont;
	std::string fontLocation;
	std::set<unsigned int> usedFontSizes;
};