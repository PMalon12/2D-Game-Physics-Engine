#pragma once
#include "Include.h"
#include "Utility.cpp"
#include "Texture.h"
#include "Camera Manager.h"
#include "Colour.h"
#include "Text.h"
#include "Font.h"
#include "Rectangle.h"

class CallbackCollection;

class Menu
{
public:
	Menu() {}
	~Menu() {}

	void init();

	void update(Time dt);
	void mouseClickCallback()
	{

	}

	void draw(float a);

	enum MenuState { Main, Play, Options, Exit, BackToGame };
	MenuState menuState;

	enum PlayMenuState { New, Load, Select };
	PlayMenuState playMenuState;

private:

	struct Line
	{
		Line() {}
		~Line() {}

		void draw();

		vec2f start, end;
	};

	struct Button
	{
		Button() : active(false) { colourTransitTime.setMicroSeconds(0); }
		~Button() {}

		void draw();
		void update();

		void click() 
		{
			if (clickFunc != nullptr && active)
			{
				clickFunc();
			}
		}

		std::function<void(void)> clickFunc;

		Text text;
		Text shadow;

		Colour defColour;
		Colour hoverColour;
		Time colourTransitTime;
		
		frect buttonBounds;

		float borderAdd;

		Time hoverStartTime;
		bool hover;

		bool active;
	};

	CallbackCollection* mainMenuCB;
	CallbackCollection* playMenuCB;
	CallbackCollection* optMenuCB;

	Colour back;
	Font* font;
	Text gameName;
	Button play;
	Button options;
	Button exit;
	RectangleShape backRect;
	Camera* menuCam;

	RectangleShape lvlSelRect;

	Line l;
	Line l2;
	Line l3, l4;

	Button newGameBut;
	Button loadGameBut;
	Button lvlSelBut;

	Line playMenu1, playMenu2, playMenu3;
	Line playMenuSelLine, playMenuSelLine2;

	//Rectangle lvlONE;

	Button lvlONE;
	Button lvlTWO;
	Button lvlTHREE;
	Button lvlFOUR;

	Button svONE;
	Button svTWO;
	Button svTHREE;
	Button svFOUR;

	Button ldONE;
	Button ldTWO;
	Button ldTHREE;
	Button ldFOUR;
};