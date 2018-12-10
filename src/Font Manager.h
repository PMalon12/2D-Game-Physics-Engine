#pragma once
#include "Include.h"
#include "Font.h"

class FontManager
{
	friend class Game;
public:
	FontManager() : initialised(false) {}
	~FontManager() {}

	void addFont(Font* font)
	{
		if (font == nullptr)
		{
			return;
		}

		if (fontMap.find(font->face->family_name) == fontMap.end())
		{
			fontMap.insert(std::make_pair(std::string(font->face->family_name), font));
		}
	}

	Font* getFont(std::string fontName)
	{
		return fontMap.find(fontName)->second;
	}

	bool isInitialised() const { return initialised; }

private:

	void init()
	{
		std::cout << "Font manager initialised" << std::endl;
		initialised = true;

		std::cout << "** Loading fonts **" << std::endl;

		std::string fontsPath("res/fnt/");

		Font* font = new Font();
		font->loadFromFile(std::string(fontsPath + "segoeuil.ttf"));
		fontMap.insert(std::make_pair(std::string("segoeuil"), font));
		fontIDMap.insert(std::make_pair(0x01010101, font));

		font = new Font();
		font->loadFromFile(std::string(fontsPath + "seguisb.ttf"));
		fontMap.insert(std::make_pair(std::string("seguisb"), font));
		fontIDMap.insert(std::make_pair(0x02020202, font));

		font = new Font();
		font->loadFromFile(std::string(fontsPath + "consola.ttf"));
		fontMap.insert(std::make_pair(std::string("consola"), font));
		fontIDMap.insert(std::make_pair(0x03030303, font));

		font = new Font();
		font->loadFromFile(std::string(fontsPath + "boltssf.ttf"));
		fontMap.insert(std::make_pair(std::string("boltssf"), font));
		fontIDMap.insert(std::make_pair(0x04040404, font));

		font = new Font();
		font->loadFromFile(std::string(fontsPath + "xiomara.ttf"));
		fontMap.insert(std::make_pair(std::string("xiomara"), font));
		fontIDMap.insert(std::make_pair(0x05050505, font));

		std::cout << "** Done loading fonts **" << std::endl;
	}

	bool initialised;
	std::map<std::string, Font*> fontMap;
	std::map<INT32, Font*> fontIDMap;

};