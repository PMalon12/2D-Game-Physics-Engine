#pragma once
#include "Include.h"
#include "Camera Manager.h"
#include "Texture.h"
#include "Colour.h"
#include "Rectangle.h"

class GameUI
{
public:
	GameUI() {}
	~GameUI() {}

	void init();

	void draw();

	void update();

private:

	RectangleShape hp;
	RectangleShape hpBar;
	Camera* cam;
	RectangleShape itemSelect;
};