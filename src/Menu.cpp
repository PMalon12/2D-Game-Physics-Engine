#include "Menu.h"
#include "Game.h"
#include "Event Manager.h"

void Menu::init()
{
	font = Game::fontManager.getFont("boltssf");
	gameName.setFont(font);
	play.text.setFont(font); 
	options.text.setFont(font); 
	exit.text.setFont(font);
	gameName.setCharSize(38); 
	play.text.setCharSize(32); 
	options.text.setCharSize(32); 
	exit.text.setCharSize(32);
	gameName.setPosition(vec2f(100, Game::ScreenSizeY - 100));
	play.text.setPosition(vec2f(150, Game::ScreenSizeY - 300));
	options.text.setPosition(vec2f(150, Game::ScreenSizeY - 400));
	exit.text.setPosition(vec2f(150, Game::ScreenSizeY - 500));

	gameName.setString("TEST CHAMBER");
	play.text.setString("Play");
	options.text.setString("Options");
	exit.text.setString("Exit");

	Colour defC = Colour(241, 102, 0);
	gameName.setColour(Colour(241, 68, 0));
	play.defColour = defC;
	options.defColour = defC;
	exit.defColour = defC;

	Colour hoverC = Colour(255, 176, 145);
	play.hoverColour = hoverC;
	options.hoverColour = hoverC;
	exit.hoverColour = hoverC;

	play.colourTransitTime = Time().setSeconds(0.1f);
	options.colourTransitTime = Time().setSeconds(0.1f);
	exit.colourTransitTime = Time().setSeconds(0.1f);

	play.borderAdd = 10;
	options.borderAdd = 10;
	exit.borderAdd = 10;

	Colour shadowC = Colour(229, 27, 27);
	play.shadow = play.text;
	play.shadow.setPosition(play.shadow.getPosition() + vec2f(1, -1));
	play.shadow.setColour(shadowC);
	play.update();

	options.shadow = options.text;
	options.shadow.setPosition(options.shadow.getPosition() + vec2f(1, -1));
	options.shadow.setColour(shadowC);
	options.update();

	exit.shadow = exit.text;
	exit.shadow.setPosition(exit.shadow.getPosition() + vec2f(1, -1));
	exit.shadow.setColour(shadowC);
	exit.update();

	l.start = vec2f(play.buttonBounds.left + play.buttonBounds.width, play.buttonBounds.bot + (play.buttonBounds.height / 2.f));
	l.end = vec2f(500, play.buttonBounds.bot + (play.buttonBounds.height / 2.f));

	l2.start = vec2f(500, play.buttonBounds.bot + (play.buttonBounds.height / 2.f) + 150);
	l2.end = vec2f(500, play.buttonBounds.bot + (play.buttonBounds.height / 2.f) - 250);

	l3.start = vec2f(500, play.buttonBounds.bot + (play.buttonBounds.height / 2.f) + 150);
	l3.end = vec2f(Game::ScreenSizeX, play.buttonBounds.bot + (play.buttonBounds.height / 2.f) + 150);

	l4.start = vec2f(500, play.buttonBounds.bot + (play.buttonBounds.height / 2.f) - 250);
	l4.end = vec2f(Game::ScreenSizeX, play.buttonBounds.bot + (play.buttonBounds.height / 2.f) - 250);

	backRect.init(frect(0, 0, Game::ScreenSizeX, Game::ScreenSizeY), Game::textureManager.getTexture("d"));
	backRect.c = Colour(0.05, 0.05, 0.05, 0.5);
	lvlSelRect.init(frect(510, play.buttonBounds.bot + (play.buttonBounds.height / 2.f) - 240, Game::ScreenSizeX - 500, 380), Game::textureManager.getTexture("d"));
	lvlSelRect.c = Colour(26, 26, 26);

	const Colour playMenuButC(101, 166, 255);
	const Colour playMenuButHC(214, 231, 255);

	newGameBut.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	newGameBut.text.setFont(font);
	newGameBut.text.setCharSize(30);
	newGameBut.text.setString("New Game");
	newGameBut.text.setPosition(vec2f(lvlSelRect.rectangle.left + 60, lvlSelRect.rectangle.top() - 35));
	newGameBut.defColour = playMenuButC;
	newGameBut.hoverColour = playMenuButHC;
	newGameBut.borderAdd = 5;
	newGameBut.colourTransitTime.setSeconds(0.1f);
	newGameBut.update();

	loadGameBut.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	loadGameBut.text.setFont(font);
	loadGameBut.text.setCharSize(30);
	loadGameBut.text.setString("Load Game");
	loadGameBut.text.setPosition(vec2f(lvlSelRect.rectangle.left + (lvlSelRect.rectangle.width / 2.f) - (loadGameBut.text.getLocalBounds().width / 2.f), lvlSelRect.rectangle.top() - 35));
	loadGameBut.defColour = playMenuButC;
	loadGameBut.hoverColour = playMenuButHC;
	loadGameBut.borderAdd = 5;
	loadGameBut.colourTransitTime.setSeconds(0.1f);
	loadGameBut.update();

	lvlSelBut.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	lvlSelBut.text.setFont(font);
	lvlSelBut.text.setCharSize(30);
	lvlSelBut.text.setString("Select Level");
	lvlSelBut.text.setPosition(vec2f(Game::ScreenSizeX - loadGameBut.text.getGlobalBounds().width - 75, lvlSelRect.rectangle.top() - 35));
	lvlSelBut.defColour = playMenuButC;
	lvlSelBut.hoverColour = playMenuButHC;
	lvlSelBut.borderAdd = 5;
	lvlSelBut.colourTransitTime.setSeconds(0.1f);
	lvlSelBut.update();

	playMenu1.start = vec2f(520, lvlSelRect.rectangle.bot + 10);
	playMenu1.end = vec2f(520, lvlSelRect.rectangle.top() - 60);

	playMenu2.start = vec2f(Game::ScreenSizeX - 10, lvlSelRect.rectangle.bot + 10);
	playMenu2.end = vec2f(Game::ScreenSizeX - 10, lvlSelRect.rectangle.top() - 60);

	playMenu3.start = vec2f(520, lvlSelRect.rectangle.top() - 60);
	playMenu3.end = vec2f(Game::ScreenSizeX - 10, lvlSelRect.rectangle.top() - 60);

	playMenuSelLine.start = vec2f(newGameBut.buttonBounds.left + (newGameBut.buttonBounds.width / 2.f), newGameBut.buttonBounds.bot);
	playMenuSelLine.end = vec2f(newGameBut.buttonBounds.left + (newGameBut.buttonBounds.width / 2.f), lvlSelRect.rectangle.top() - 60);

	playMenuSelLine2.start = vec2f(newGameBut.buttonBounds.left , newGameBut.buttonBounds.bot);
	playMenuSelLine2.end = vec2f(newGameBut.buttonBounds.left + newGameBut.buttonBounds.width, newGameBut.buttonBounds.bot);

	

	const Colour lvlC = Colour(0.9, 0.9, 0.9, 1.0);
	const Colour lvlHC = Colour(0.9, 0.9, 0.55, 1.0);
	lvlONE.text.setFont(font);
	lvlONE.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	lvlONE.text.setCharSize(26);
	lvlONE.text.setPosition(vec2f(540, playMenu3.start.y - 40));
	lvlONE.text.setString("LEVEL ONE");
	lvlONE.defColour = lvlC;
	lvlONE.hoverColour = lvlHC;
	lvlONE.borderAdd = 4;
	lvlONE.colourTransitTime.setSeconds(0.1f);
	lvlONE.update();

	lvlTWO.text.setFont(font);
	lvlTWO.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	lvlTWO.text.setCharSize(26);
	lvlTWO.text.setPosition(vec2f(540, playMenu3.start.y - 80));
	lvlTWO.text.setString("LEVEL TWO");
	lvlTWO.defColour = lvlC;
	lvlTWO.hoverColour = lvlHC;
	lvlTWO.borderAdd = 4;
	lvlTWO.colourTransitTime.setSeconds(0.1f);
	lvlTWO.update();

	lvlTHREE.text.setFont(font);
	lvlTHREE.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	lvlTHREE.text.setCharSize(26);
	lvlTHREE.text.setPosition(vec2f(540, playMenu3.start.y - 120));
	lvlTHREE.text.setString("LEVEL THREE");
	lvlTHREE.defColour = lvlC;
	lvlTHREE.hoverColour = lvlHC;
	lvlTHREE.borderAdd = 4;
	lvlTHREE.colourTransitTime.setSeconds(0.1f);
	lvlTHREE.update();

	lvlFOUR.text.setFont(font);
	lvlFOUR.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	lvlFOUR.text.setCharSize(26);
	lvlFOUR.text.setPosition(vec2f(540, playMenu3.start.y - 160));
	lvlFOUR.text.setString("LEVEL FOUR");
	lvlFOUR.defColour = lvlC;
	lvlFOUR.hoverColour = lvlHC;
	lvlFOUR.borderAdd = 4;
	lvlFOUR.colourTransitTime.setSeconds(0.1f);
	lvlFOUR.update();

	const Colour svC = Colour(0.9, 0.9, 0.9, 1.0);
	const Colour svHC = Colour(0.9, 0.9, 0.55, 1.0);
	svONE.text.setFont(font);
	svONE.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	svONE.text.setCharSize(26);
	svONE.text.setString("SAVE ONE");
	svONE.text.setPosition(vec2f(520 + ((Game::ScreenSizeX - 10 - 520) / 3.f) - (svONE.text.getLocalBounds().width / 2), playMenu3.start.y - 40));
	svONE.defColour = svC;
	svONE.hoverColour = svHC;
	svONE.borderAdd = 4;
	svONE.colourTransitTime.setSeconds(0.1f);
	svONE.update();

	svTWO.text.setFont(font);
	svTWO.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	svTWO.text.setCharSize(26);
	svTWO.text.setString("SAVE TWO");
	svTWO.text.setPosition(vec2f( 520 + ((Game::ScreenSizeX - 10 - 520) * 2.f / 3.f) -(svTWO.text.getLocalBounds().width/2), playMenu3.start.y - 40));
	svTWO.defColour = svC;
	svTWO.hoverColour = svHC;
	svTWO.borderAdd = 4;
	svTWO.colourTransitTime.setSeconds(0.1f);
	svTWO.update();

	svTHREE.text.setFont(font);
	svTHREE.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	svTHREE.text.setCharSize(26);
	svTHREE.text.setString("SAVE THREE");
	svTHREE.text.setPosition(vec2f(520 + ((Game::ScreenSizeX - 10 - 520) / 3.f) -(svTHREE.text.getLocalBounds().width/2), playMenu3.start.y - 190));
	svTHREE.defColour = svC;
	svTHREE.hoverColour = svHC;
	svTHREE.borderAdd = 4;
	svTHREE.colourTransitTime.setSeconds(0.1f);
	svTHREE.update();

	svFOUR.text.setFont(font);
	svFOUR.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	svFOUR.text.setCharSize(26);
	svFOUR.text.setString("SAVE FOUR");
	svFOUR.text.setPosition(vec2f(520 + ((Game::ScreenSizeX - 10 - 520) * 2.f / 3.f) -(svFOUR.text.getLocalBounds().width/2), playMenu3.start.y - 190));
	svFOUR.defColour = svC;
	svFOUR.hoverColour = svHC;
	svFOUR.borderAdd = 4;
	svFOUR.colourTransitTime.setSeconds(0.1f);
	svFOUR.update();

	const Colour ldC = Colour(0.9, 0.9, 0.9, 1.0);
	const Colour ldHC = Colour(0.9, 0.9, 0.55, 1.0);
	ldONE.text.setFont(font);
	ldONE.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	ldONE.text.setCharSize(26);
	ldONE.text.setString("SAVE ONE");
	ldONE.text.setPosition(vec2f(520 + ((Game::ScreenSizeX - 10 - 520) / 3.f) - (ldONE.text.getLocalBounds().width / 2), playMenu3.start.y - 40));
	ldONE.defColour = ldC;
	ldONE.hoverColour = ldHC;
	ldONE.borderAdd = 4;
	ldONE.colourTransitTime.setSeconds(0.1f);
	ldONE.update();

	ldTWO.text.setFont(font);
	ldTWO.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	ldTWO.text.setCharSize(26);
	ldTWO.text.setString("SAVE TWO");
	ldTWO.text.setPosition(vec2f(520 + ((Game::ScreenSizeX - 10 - 520) * 2.f / 3.f) - (ldTWO.text.getLocalBounds().width / 2), playMenu3.start.y - 40));
	ldTWO.defColour = ldC;
	ldTWO.hoverColour = ldHC;
	ldTWO.borderAdd = 4;
	ldTWO.colourTransitTime.setSeconds(0.1f);
	ldTWO.update();

	ldTHREE.text.setFont(font);
	ldTHREE.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	ldTHREE.text.setCharSize(26);
	ldTHREE.text.setString("SAVE THREE");
	ldTHREE.text.setPosition(vec2f(520 + ((Game::ScreenSizeX - 10 - 520) / 3.f) - (ldTHREE.text.getLocalBounds().width / 2), playMenu3.start.y - 190));
	ldTHREE.defColour = ldC;
	ldTHREE.hoverColour = ldHC;
	ldTHREE.borderAdd = 4;
	ldTHREE.colourTransitTime.setSeconds(0.1f);
	ldTHREE.update();

	ldFOUR.text.setFont(font);
	ldFOUR.text.setColour(Colour(0.9, 0.9, 0.9, 1.0));
	ldFOUR.text.setCharSize(26);
	ldFOUR.text.setString("SAVE FOUR");
	ldFOUR.text.setPosition(vec2f(520 + ((Game::ScreenSizeX - 10 - 520) * 2.f / 3.f) - (ldFOUR.text.getLocalBounds().width / 2), playMenu3.start.y - 190));
	ldFOUR.defColour = ldC;
	ldFOUR.hoverColour = ldHC;
	ldFOUR.borderAdd = 4;
	ldFOUR.colourTransitTime.setSeconds(0.1f);
	ldFOUR.update();

	static auto playClick = [this](void) -> void { this->menuState = Menu::Play; };
	play.clickFunc = playClick;

	static auto optClick = [this](void) -> void { this->menuState = Menu::Options; };
	options.clickFunc = optClick;

	static auto exitClick = [this](void) -> void { this->menuState = Menu::Exit; };
	exit.clickFunc = exitClick;

	static auto mainMenuClick = [this](void*) -> void 
	{
		auto mouse = Game::getMousePos();
		
		if (this->play.buttonBounds.contains(mouse))
			this->play.click();

		if (this->options.buttonBounds.contains(mouse))
			this->options.click();

		if (this->exit.buttonBounds.contains(mouse))
			this->exit.click();
	};

	static auto newGameClick = [this](void) -> void { this->playMenuState = Menu::New; };
	newGameBut.clickFunc = newGameClick;

	static auto loadGameClick = [this](void) -> void { this->playMenuState = Menu::Load; };
	loadGameBut.clickFunc = loadGameClick;

	static auto lvlSelClick = [this](void) -> void { this->playMenuState = Menu::Select; };
	lvlSelBut.clickFunc = lvlSelClick;


	static auto lvlONEClick = [this](void) -> void {Game::cameraManager.setCurrCamera(1); Game::map->loadMap("ONE"); this->menuState = BackToGame; };
	lvlONE.clickFunc = lvlONEClick;

	static auto lvlTWOClick = [this](void) -> void {Game::cameraManager.setCurrCamera(1); Game::map->loadMap("TWO"); this->menuState = BackToGame; };
	lvlTWO.clickFunc = lvlTWOClick;

	static auto playMenuClick = [this](void*) -> void
	{
		auto mouse = Game::getMousePos();

		if (!this->lvlSelRect.rectangle.contains(mouse))
			this->menuState = Menu::Main;

		if (this->newGameBut.buttonBounds.contains(mouse))
			this->newGameBut.click();

		if (this->loadGameBut.buttonBounds.contains(mouse))
			this->loadGameBut.click();

		if (this->lvlSelBut.buttonBounds.contains(mouse))
			this->lvlSelBut.click();

		if (playMenuState == Select)
		{
			if (this->lvlONE.buttonBounds.contains(mouse))
				this->lvlONE.click();

			if (this->lvlTWO.buttonBounds.contains(mouse))
				this->lvlTWO.click();
		}
	};

	//static auto lvlOneClick = [this](void) -> void {}

	mainMenuCB = new CallbackCollection;
	playMenuCB = new CallbackCollection;
	optMenuCB = new CallbackCollection;

	sf::Event::MouseButtonEvent a; a.button = sf::Mouse::Left;
	mainMenuCB->addMousePressCallback(mainMenuClick, MouseEventWrap(a), &play, 0);
	playMenuCB->addMousePressCallback(playMenuClick, MouseEventWrap(a), this, 0);

	Game::eventManager.setCurrentCallbackSet(mainMenuCB);
	menuState = Main;
}

void Menu::Button::update()
{
	auto mouse = Game::getMousePos();

	frect textBorder = text.getGlobalBounds();
	buttonBounds = frect(textBorder.left - borderAdd, textBorder.bot - borderAdd, textBorder.width + (2 * borderAdd), textBorder.height + (2 * borderAdd));

	if (buttonBounds.contains(mouse))
	{
		if (!hover)
		{
			hoverStartTime.setMicroSeconds(Game::tickCounter.getProgramTime());
		}
		hover = true;

		float lerpFactor = std::min(double(Game::tickCounter.getProgramTime() - hoverStartTime.getMicroSeconds()) / double(colourTransitTime.getMicroSeconds()), 1.0);

		text.setColour(Colour(clerp(defColour, hoverColour, lerpFactor)));
	}
	else
	{
		if (hover)
		{
			hoverStartTime.setMicroSeconds(Game::tickCounter.getProgramTime());
		}
		hover = false;

		float lerpFactor = std::min(double(Game::tickCounter.getProgramTime() - hoverStartTime.getMicroSeconds()) / double(colourTransitTime.getMicroSeconds()), 1.0);

		text.setColour(Colour(clerp(hoverColour, defColour, lerpFactor)));
		
	}
}

void Menu::Button::draw()
{
	shadow.draw(); text.draw();
	if (Game::getOption(DRAW_STRUCT_BOUNDS))
	{
		frect b = buttonBounds;

		auto s = Game::shaderManager.getShader("min2");
		s->use();

		auto col = glGetUniformLocation(s->getID(), "colour");
		glUniform4f(col, 1, 1, 1, 1);

		auto model = glGetUniformLocation(s->getID(), "model");
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::fmat4()));

		glBegin(GL_LINE_LOOP);

		glVertex3f(b.left, b.bot, 0);
		glVertex3f(b.left, b.bot + b.height, 0);
		glVertex3f(b.left + b.width, b.bot + b.height, 0);
		glVertex3f(b.left + b.width, b.bot, 0);

		glEnd();
	}
}

void Menu::Line::draw()
{
	auto s = Game::shaderManager.getShader("min2");
	s->use();

	auto col = glGetUniformLocation(s->getID(), "colour");
	glUniform4f(col, 1, 1, 1, 1);

	auto model = glGetUniformLocation(s->getID(), "model");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::fmat4()));

	glBegin(GL_LINES);

	glVertex3f(start.x, start.y, 0);
	glVertex3f(end.x, end.y, 0);

	glEnd();
}

void Menu::update(Time dt)
{
	switch (menuState)
	{
	case(Main) : {

		play.active = true;
		options.active = true;
		exit.active = true;

		newGameBut.active = false;
		loadGameBut.active = false;
		lvlSelBut.active = false;

		Game::eventManager.setCurrentCallbackSet(mainMenuCB);
		play.update();
		options.update();
		exit.update();

		break; }
	case(Play) : {

		const Colour playMenuButC(101, 166, 255);
		const Colour playMenuButHC(214, 231, 255);

		play.active = false;
		options.active = false;
		exit.active = false;

		newGameBut.active = true;
		loadGameBut.active = true;
		lvlSelBut.active = true;

		switch (playMenuState)
		{
		case(New) : {
			newGameBut.defColour = playMenuButHC;
			loadGameBut.defColour = playMenuButC;
			lvlSelBut.defColour = playMenuButC;

			lvlONE.active = false;
			lvlTWO.active = false;
			lvlTHREE.active = false;
			lvlFOUR.active = false;

			svONE.active = true;
			svTWO.active = true;
			svTHREE.active = true;
			svFOUR.active = true;

			ldONE.active = false;
			ldTWO.active = false;
			ldTHREE.active = false;
			ldFOUR.active = false;

			playMenuSelLine.start = vec2f(newGameBut.buttonBounds.left + (newGameBut.buttonBounds.width / 2.f), newGameBut.buttonBounds.bot);
			playMenuSelLine.end = vec2f(newGameBut.buttonBounds.left + (newGameBut.buttonBounds.width / 2.f), lvlSelRect.rectangle.top() - 60);

			playMenuSelLine2.start = vec2f(newGameBut.buttonBounds.left, newGameBut.buttonBounds.bot);
			playMenuSelLine2.end = vec2f(newGameBut.buttonBounds.left + newGameBut.buttonBounds.width, newGameBut.buttonBounds.bot);

			svONE.update();
			svTWO.update();
			svTHREE.update();
			svFOUR.update();

			break; }
		case(Load) : {
			newGameBut.defColour = playMenuButC;
			loadGameBut.defColour = playMenuButHC;
			lvlSelBut.defColour = playMenuButC;

			lvlONE.active = false;
			lvlTWO.active = false;
			lvlTHREE.active = false;
			lvlFOUR.active = false;

			svONE.active = false;
			svTWO.active = false;
			svTHREE.active = false;
			svFOUR.active = false;

			ldONE.active = true;
			ldTWO.active = true;
			ldTHREE.active = true;
			ldFOUR.active = true;

			playMenuSelLine.start = vec2f(loadGameBut.buttonBounds.left + (loadGameBut.buttonBounds.width / 2.f), loadGameBut.buttonBounds.bot);
			playMenuSelLine.end = vec2f(loadGameBut.buttonBounds.left + (loadGameBut.buttonBounds.width / 2.f), lvlSelRect.rectangle.top() - 60);

			playMenuSelLine2.start = vec2f(loadGameBut.buttonBounds.left, loadGameBut.buttonBounds.bot);
			playMenuSelLine2.end = vec2f(loadGameBut.buttonBounds.left + loadGameBut.buttonBounds.width, loadGameBut.buttonBounds.bot);

			ldONE.update();
			ldTWO.update();
			ldTHREE.update();
			ldFOUR.update();

			break; }
		case(Select) : {
			newGameBut.defColour = playMenuButC;
			loadGameBut.defColour = playMenuButC;
			lvlSelBut.defColour = playMenuButHC;

			lvlONE.active = true;
			lvlTWO.active = true;
			lvlTHREE.active = true;
			lvlFOUR.active = true;

			svONE.active = false;
			svTWO.active = false;
			svTHREE.active = false;
			svFOUR.active = false;

			ldONE.active = false;
			ldTWO.active = false;
			ldTHREE.active = false;
			ldFOUR.active = false;

			playMenuSelLine.start = vec2f(lvlSelBut.buttonBounds.left + (lvlSelBut.buttonBounds.width / 2.f), lvlSelBut.buttonBounds.bot);
			playMenuSelLine.end = vec2f(lvlSelBut.buttonBounds.left + (lvlSelBut.buttonBounds.width / 2.f), lvlSelRect.rectangle.top() - 60);

			playMenuSelLine2.start = vec2f(lvlSelBut.buttonBounds.left, lvlSelBut.buttonBounds.bot);
			playMenuSelLine2.end = vec2f(lvlSelBut.buttonBounds.left + lvlSelBut.buttonBounds.width, lvlSelBut.buttonBounds.bot);

			lvlONE.update();
			lvlTWO.update();
			lvlTHREE.update();
			lvlFOUR.update();

			break; }
		}



		Game::eventManager.setCurrentCallbackSet(playMenuCB);

		lvlSelBut.update();
		loadGameBut.update();
		newGameBut.update();

		break; }
	case(Options) : {

		lvlONE.active = false;
		lvlTWO.active = false;
		lvlTHREE.active = false;
		lvlFOUR.active = false;

		svONE.active = false;
		svTWO.active = false;
		svTHREE.active = false;
		svFOUR.active = false;

		ldONE.active = false;
		ldTWO.active = false;
		ldTHREE.active = false;
		ldFOUR.active = false;

		newGameBut.active = false;
		loadGameBut.active = false;
		lvlSelBut.active = false;

		play.active = false;
		options.active = false;
		exit.active = false;

		Game::eventManager.setCurrentCallbackSet(optMenuCB);

		break; }
	case(Exit) : {
		Game::gameState = Game::Exiting;
		break; }
	}
}

void Menu::draw(float a)
{
	backRect.draw();
	gameName.draw();
	play.draw();
	options.draw();
	exit.draw();

	switch (menuState)
	{
	case(Main) : {

		break; }
	case(Play) : {
		Colour tmp; tmp = backRect.c; backRect.c = Colour(0.0, 0.0, 0.0, 0.5);
		backRect.draw();
		l.draw(); l2.draw(); l3.draw(); l4.draw();
		backRect.c = tmp;
		lvlSelRect.draw();
		lvlSelBut.draw();
		loadGameBut.draw();
		newGameBut.draw();
		playMenu1.draw();
		playMenu2.draw();
		playMenu3.draw();
		playMenuSelLine.draw(); playMenuSelLine2.draw();

		switch (playMenuState)
		{
		case(New) : {
			svONE.draw();
			svTWO.draw();
			svTHREE.draw();
			svFOUR.draw();

			break; }
		case(Load) : {

			ldONE.draw();
			ldTWO.draw();
			ldTHREE.draw();
			ldFOUR.draw();

			break; }
		case(Select) : {

			lvlONE.draw();
			lvlTWO.draw();
			lvlTHREE.draw();
			lvlFOUR.draw();
			break; }
		}
		break; }
	case(Options) : {

		break; }
	case(Exit) : {

		break; }
	}


}