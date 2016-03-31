#pragma once
#include "Include.h"
#include "Texture.h"

class TextureManager
{
public:
	TextureManager()
	{
		//Texture* temp = new Texture();
		//temp->loadTexture("b.png");
		//textures.insert(std::pair<std::string, Texture*>(std::string("b"), temp));
		//textures.insert(std::make_pair(std::string("backTexture"), new Texture("back.png")));
		//textures.insert(std::pair<std::string, Texture*>("char", new Texture("char.png")));
		//textures.insert(std::pair<std::string, Texture*>("glow", new Texture("glow.png")));
		//textures.insert(std::pair<std::string, Texture*>("pmag", new Texture("pmag.png")));
		//textures.insert(std::pair<std::string, Texture*>("lava", new Texture("lava.png")));
	}

	~TextureManager()
	{
		for (auto i = textures.begin(); i != textures.end(); ++i)
		{
			delete i->second;
		}
		textures.clear();
	}

	void init()
	{
		addTexture("circ");
		addTexture("carbon");
		addTexture("backTexture", "back.png");
		addTexture("char");
		addTexture("glow");
		addTexture("pmag");
		addTexture("lava");
		addTexture("UIHP");
		addTexture("hlight");
		addTexture("backo");
		addTexture("push");

		float t[] = { 1.f, 1.f, 1.f, 1.f };
		auto te = new Texture();
		te->loadFromMem(t, 1, 1);
		textures.insert(std::make_pair(std::string("d"), te));
		idTextures.insert(std::make_pair(textures.size() - 1, te));
	}

	Texture* getTexture(std::string pTextureName)
	{
		TexName n(pTextureName);
		auto texItr = textures.find(pTextureName);
		if (texItr != textures.end())
			return texItr->second;
		return nullptr;
	}

	Texture* getTexture(int32_t pTextureID)
	{
		auto texItr = idTextures.find(pTextureID);
		if (texItr != idTextures.end())
			return texItr->second;
		return nullptr;
	}

	void addTexture(std::string texName, std::string texPath)
	{
		auto nt = new Texture("res/tex/" + texPath, textures.size()); nt->setName(texName.c_str());
		textures.insert(std::make_pair(texName, nt));
		idTextures.insert(std::make_pair(textures.size() - 1, nt));
	}

	void addTexture(std::string texName)
	{
		auto nt = new Texture("res/tex/" + texName + ".png", textures.size()); nt->setName(texName.c_str());
		textures.insert(std::make_pair(texName, nt));
		idTextures.insert(std::make_pair(textures.size() - 1, nt));
	}

private:

	struct TexName
	{
		TexName() { for (int i = 0; i < 15; ++i) name[i] = '\0'; }
		TexName(std::string n)
		{
			for (int i = 0; i < 15; ++i) name[i] = '\0';
			std::memcpy(name, n.c_str(), std::min(n.size(), size_t(15)));
		}
		char name[16];

		bool operator<(const TexName& lhs) const
		{
			std::string lhsstr(lhs.name, 16);
			std::string rhsstr(name, 16);

			return lhsstr < rhsstr;
		}
	};

	std::map<std::string, Texture*> textures;
	std::map<int32_t, Texture*> idTextures;
	int32_t topID;
};