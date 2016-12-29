#include "GameUI.h"
#include "Game.h"

void GameUI::init()
{
	auto tex = Game::textureManager.getTexture("UIHP");
	hp.init(frect(0, 0, tex->getWidth(), tex->getHeight()), tex);
	hp.c = Colour(1.0, 1.0, 1.0, 1.0);

	hpBar.init(frect(56, 80, 166, 15), Game::textureManager.getTexture("d"));
	hpBar.c = Colour(1.0, 0.0, 0.0, 1.0);

	itemSelect.init(frect(34, 34, 34, 34), Game::textureManager.getTexture("hlight"));
	itemSelect.c = Colour(1.0, 0.0, 0.0, 1.0);
}

void GameUI::draw()
{
	Game::cameraManager.setCurrCamera("DefaultCamera");
	hp.draw();
	hpBar.draw();
	itemSelect.draw();
	Game::cameraManager.setCurrCamera(Game::map->getCurrentCamera());
}

void GameUI::update()
{
	if (hpBar.rectangle.width != Game::map->getPlayer()->getHealth() * 0.01f * 166)
	{
		hpBar.rectangle.width = Game::map->getPlayer()->getHealth() * 0.01f * 166;
		hpBar.updateVBO();
	}

	float xItemSelect = 34 + ((Game::map->getPlayer()->getCurItem() - 1) * 39);

	if (Game::map->getPlayer()->getCurItem() < 1)
	{
		xItemSelect = -100;
	}

	if (itemSelect.rectangle.left != xItemSelect)
	{
		itemSelect.rectangle.left = xItemSelect;
		itemSelect.updateVBO();
	}
}