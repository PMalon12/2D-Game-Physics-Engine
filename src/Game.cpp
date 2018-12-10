#pragma once
#include "Game.h"
#include "Text.h"
#include "GameUI.h"
#include <memory>
#include "Console.h"

//#define SetFunc1Var(func) ConsoleSetFunc(new std::function<void(double)>(func))
//#define SetFunc2Var(func) ConsoleSetFunc(new std::function<void(vec2d)>(func))
//#define SetFunc3Var(func) ConsoleSetFunc(new std::function<void(vec3d)>(func))
//#define SetFunc4Var(func) ConsoleSetFunc(new std::function<void(vec4d)>(func))

#include <chrono>

DynamicStructure* createBall(vec2f pos, float rad, int n)
{
	int num = n;
	Triangle* trisss = new Triangle[num];

	//trisss[0].setVertices(Vertex(-0.5 , -0.5), Vertex(0.5, 0.5), Vertex(-0.5, 0.5));
	//trisss[1].setVertices(Vertex(-0.5, -0.5), Vertex(0.5, -0.5), Vertex(0.5, 0.5));

	//float* edgeVerts = new float[num * 2];

	for (int i = 0; i < num; ++i)
	{
		vec2f nextA(rad*std::sin((-i*PI * 2) / num), rad*std::cos((-i*PI * 2) / num));
		vec2f nextB(rad*std::sin(((-i + 1)*PI * 2) / num), rad*std::cos(((-i + 1)*PI * 2) / num));
		vec2f nAN(nextA); nAN.normalise(); nAN = nAN * 0.5;
		vec2f nBN(nextB); nBN.normalise(); nBN = nBN * 0.5;
		trisss[i].setVertices(Vertex2D(0, 0, 0, 0), 
			Vertex2D(nextA.x, nextA.y, nAN.x, nAN.y), 
			Vertex2D(nextB.x, nextB.y, nBN.x, nBN.y));
		//edgeVerts[2 * i] = rad*std::sin((-i*PI * 2) / num); edgeVerts[(2 * i) + 1] = rad*std::cos((-i*PI * 2) / num);
	}

	TriangleMesh* m = new TriangleMesh(num, trisss);

	DynamicStructure* tmp = new DynamicStructure(m);
	tmp->setBodyMass(200.f);
	tmp->staticFriction = 0.11;
	tmp->dynamicFriction = 0.1;
	tmp->restitution = 0.0;

	tmp->setPosition(vec2f(pos));
	tmp->setBodyPosition(vec2f(pos)); 

	float e = rad;

	float ia = tmp->getBodyMass() * (std::pow(e, 2) + std::pow(e, 2)) / 12.f;

	tmp->setBodyInertiaMoment(ia);

	tmp->mesh->buildConvexHull();

	return tmp;
}

void Game::start(void)
{
	sf::ContextSettings Settings;
	Settings.depthBits = 24;
	Settings.stencilBits = 8;
	Settings.antialiasingLevel = 16;

	ScreenSizeX = 1920;
	ScreenSizeY = 1080;

	//auto vidMode = (*sf::VideoMode::getFullscreenModes().begin());
	mainWindow.create(sf::VideoMode(ScreenSizeX, ScreenSizeY), "AH Project", sf::Style::Fullscreen, Settings);
	//mainWindow.create(vidMode, "AH Project", sf::Style::Fullscreen, Settings);
	//ScreenSizeX = vidMode.width;
	//ScreenSizeY = vidMode.height;
	mainWindow.setVerticalSyncEnabled(true);

	static std::function<void(float)> setWire = [](float val) -> void { setOption(DRAW_WIREFRAME, int(val)); if (int(val) == 0) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); } else { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); } };

	Console::addSetter("drawWire", new SetFunc1Var(setWire));
	Console::addSetter("dontGetDizzy", new SetFunc1Var(setRotate));

	GLint maj, min;
	glGetIntegerv(GL_MAJOR_VERSION, &maj);
	glGetIntegerv(GL_MINOR_VERSION, &min);

	std::cout << "GL version " << maj << "." << min << std::endl;

	auto glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "GLSL version " << glsl << std::endl;

	std::cout << "Screen Resolution: " << ScreenSizeX << "x" << ScreenSizeY << std::endl;

	gameState = ShowingMenu;

	debugOptions[0] = 0;
	debugOptions[1] = 0;
	debugOptions[2] = 0;

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureManager.init();
	shaderManager.init();
	cameraManager.init();
	fTManager.init();
	fontManager.init();
	debugOverlay.init();

	cameraManager.addCamera(std::string("playerCam"));
	cameraManager.setCurrCamera(1);
	CURR_CAM->setMaxZoom(0.5);
	CURR_CAM->setMinZoom(6);
	CURR_CAM->setSmoothFollow(true);
	CURR_CAM->setFollowSpeed(20);
	CURR_CAM->setActive();
	CURR_CAM->setConstrainCam(true);

	cameraManager.addCamera("editCam");
	cameraManager.setCurrCamera(2);
	CURR_CAM->removeZoomConstraint();
	CURR_CAM->setSmoothFollow(false);
	CURR_CAM->setConstrainCam(false);
	std::unique_ptr<NullEntity> camEntity(new NullEntity);
	CURR_CAM->setFollowTarget(camEntity.get());

	cameraManager.setCurrCamera(1);

	vec2f mapSize(10000, 10000);
	map = new Map(mapSize);

	CURR_CAM->setContraintRect(frect(0, 0, map->properties.size.x, map->properties.size.y));

	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());

	static const auto setpGran = [](vec2d val) -> void { Game::map->placeGranularity = val; };
	static const auto setsGran = [](vec2d val) -> void { Game::map->scaleGranularity = val; };
	static const auto setrGran = [](double val) -> void { Game::map->rotateGranularity = val; };
	static const auto sethp = [](double val) -> void { Game::map->getPlayer()->setHealth(val); };
	static const auto cc = [](double val) -> void { Game::map->resetCheckpointInfo(); };
	static const auto dui = [](double val) -> void { Game::drawUI = bool(val); };

	Console::addSetter("pgran", new SetFunc2Var(setpGran));
	Console::addSetter("sgran", new SetFunc2Var(setsGran));
	Console::addSetter("rgran", new SetFunc1Var(setrGran));
	Console::addSetter("camSmooth", new SetFunc1Var(std::bind(&Camera::setSmoothFollow, Game::cameraManager.getCurrCamera(), std::placeholders::_1)));
	Console::addSetter("grav", new SetFunc2Var(Game::setGrav));
	Console::addSetter("iter", new SetFunc1Var(Game::setIter));
	Console::addSetter("opt", new SetFunc2Var(Game::setOptionConsole));
	Console::addSetter("objTex", new SetFunc3Var(Game::setTex));
	Console::addSetter("remove", new SetFunc1Var(Game::removeObj));
	Console::addSetter("checkpoint", new SetFunc2Var(Game::setCheckpointIndex));
	Console::addSetter("hp", new SetFunc1Var(sethp));
	Console::addSetter("clearchecks", new SetFunc1Var(cc));
	Console::addSetter("texClip", new SetFunc2Var(Game::setTexClip));
	Console::addSetter("objCol", new SetFunc5Var(Game::setObjCol));
	Console::addSetter("backTex", new SetFunc2Var(Game::setBackTex));
	Console::addSetter("pow", new SetFunc2Var(Game::setPower));
	Console::addSetter("gravaf", new SetFunc2Var(Game::setGravAf));
	Console::addSetter("drawUI", new SetFunc1Var(dui));

	iter = 100;

	tickCounter.initialise();

	static auto rotadd = [](void* _this) -> void { rot += (Game::getDT().getSeconds() * 0.4f); };
	static auto rotsub = [](void* _this) -> void { rot += (Game::getDT().getSeconds() * -0.4f); };

	Game::cb.addKeyStateCallback(rotadd, KeyEventWrap(sf::Keyboard::B), nullptr, 0);
	Game::cb.addKeyStateCallback(rotsub, KeyEventWrap(sf::Keyboard::V), nullptr, 0);

	menu = new Menu();
	menu->init();

	setGrav(vec2d(0, 0));
	setBack(vec3d(0.05, 0.05, 0.05));

	setDT(Time().setMicroSeconds(5000));

	while (!isExiting())
	{
		gameLoop();
	}

	mainWindow.close();
}

bool Game::isExiting()
{
	if (gameState == Exiting)
		return true;
	else
		return false;
}

void Game::gameLoop()
{
	switch (gameState)
	{
	case Playing:
	{
		static uint64_t renderedFrames = 0;
		static int64_t currTime = 0;
		
		cameraManager.setCurrCamera(1);

		int64_t accumulator = 0;

		tickCounter.updateProgramTime();
		eventManager.setCurrentCallbackSet(&cb);

		/*int pyrNum = 10;
		int numEl = ((pyrNum*(pyrNum + 1)) / 2);
		std::vector<DynamicStructure*> balls;
		float radius = 300;
		float xS = 1010 + radius, yS = 610 + radius;
		float xP = xS, yP = yS;
		int numForRow = pyrNum;
		for (int i = 0; i < numEl; ++i)
		{
			yP = yS + (i * radius * 1.5f);
			for (int j = 0; j < numForRow; ++j)
			{
				xP = xS + (j * radius * 1.5f);
				balls.push_back(createBall(vec2f(xP, yP), radius,4 (rng() % 9) + 3));
				balls.back()->setRotation(PI / 4); balls.back()->setBodyRotation(PI / 4);
				map->addDynamic(balls.back());
				balls.back()->setTexture(textureManager.getTexture("d"));
				balls.back()->staticFriction = 1.f;
				balls.back()->dynamicFriction = 1.f;
				auto tex = int(rng() % 4) + 0;
				balls.back()->setTexture(Game::textureManager.getTexture(tex));
				if(tex == 3)
					balls.back()->setTexture(Game::textureManager.getTexture(9));
				if(tex == 1)
					balls.back()->mesh->updateTextureCoords(0.1,vec2f(0,0));
				else
					balls.back()->mesh->updateTextureCoords(vec2f(0, 0));
				balls.back()->mesh->r = 1;
				balls.back()->mesh->g = 1;
				balls.back()->mesh->b = 1;
				balls.back()->mesh->a = 1;
				float xr = (float(rng() % 1000) / 1000.f) + 1.f;
				float yr = (float(rng() % 1000) / 1000.f) + 1.f;
				//balls.back()->setScale(vec2f(xr, yr));
			}
			xS += (radius * 1.5f) / 2;
			--numForRow;
		}*/

		

		/*
		int num = 100;
		float rad = 100;
		auto trisss = new Triangle[num];

		for (int i = 0; i < num; ++i)
		{
			vec2f nextA(rad*std::sin((-i*PI * 2) / num), rad*std::cos((-i*PI * 2) / num));
			vec2f nextB(rad*std::sin(((-i + 1)*PI * 2) / num), rad*std::cos(((-i + 1)*PI * 2) / num));
			vec2f nAN(nextA); nAN.normalise(); nAN = nAN * 0.5;
			vec2f nBN(nextB); nBN.normalise(); nBN = nBN * 0.5;
			trisss[i].setVertices(Vertex2D(0, 0, 0, 0), 
									Vertex2D(nextA.x, nextA.y, nAN.x, nAN.y), 
									Vertex2D(nextB.x, nextB.y, nBN.x, nBN.y));
		}

		auto tri = new Triangle[4];
		tri[0].setVertices(Vertex2D(100, 100), Vertex2D(200, 100), Vertex2D(150, 200));
		tri[1].setVertices(Vertex2D(150, 200), Vertex2D(200, 100), Vertex2D(300, 350));
		tri[2].setVertices(Vertex2D(200, 100), Vertex2D(600, 100), Vertex2D(300, 350));
		tri[3].setVertices(Vertex2D(150, 200), Vertex2D(100, 200), Vertex2D(125, 300));

		auto tm = new TriangleMesh(3, tri,Transform().translate(vec2f(450,500)));

		auto sss = new StaticStructure(tm);
		tm->r = 0.f; tm->g = 0.f; tm->b = 1.f; tm->a = 0.2;

		tri[0].setVertices(Vertex2D(-50, -50), Vertex2D(50, -50), Vertex2D(100, 0));

		tm = new TriangleMesh(1, tri);
		tm->r = 1.f; tm->g = 0.f; tm->b = 0.f; tm->a = 0.2;

		auto sss2 = new StaticStructure(tm);

		map->addStatic(sss);
		map->addStatic(sss2);*/

		GameUI ui; ui.init();

		while (gameState == Playing)
		{
			////////////////////////////////////////////////////// UPDATE //////////////////////////////////////////////////////

			if (mapChanged)
			{
				std::cout << "LOADING MAP: " << nextMapName << std::endl;
				map->loadMap(nextMapName);
				mapChanged = false;
			}

			tickCounter.updateProgramTime();
			int64_t newTime = tickCounter.getProgramTime();
			int64_t frameTime = newTime - currTime;
			if (frameTime > 25000)
			{
				frameTime = 25000;
			}
			currTime = newTime;

			accumulator += frameTime;

			tickCounter.updateElapsedTime();
			tickCounter.updateTicksPerSecond();

			cameraManager.setCurrCamera(map->currentCamera);

			eventManager.handleEvents();

			//sss2->setPosition(getWorldMousePos());

			while (accumulator >= delTime.getMicroSeconds())
			{
				eventManager.handleStateBased();

				tickCounter.updateProgramTime();
				auto start = tickCounter.getProgramTime();
				contacts.clear();
				map->update(delTime.getMicroSeconds());

				tickCounter.updateProgramTime();
				auto end = tickCounter.getProgramTime();

				accumulator -= delTime.getMicroSeconds();
				if (rotateD)
					CURR_CAM->rotate(delTime.getSeconds() * 0.4f);
				else
					CURR_CAM->setRotation(0);
				CURR_CAM->update(delTime);
			}

			ui.update();

			////////////////////////////////////////////////////// RENDER //////////////////////////////////////////////////////

			glClearColor(Game::back.r, Game::back.g, Game::back.b, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			double alpha = double(accumulator) / double(dt);

			polyDrawn = 0;
			obDrawn = 0;

			if (!getOption(DRAW_INTERPOLATE)) alpha = 0;

			glLineWidth(2);
			map->draw(alpha);
			if (drawUI)
			{
				ui.draw();
			}
			debugOverlay.draw();
			
			mainWindow.display();
		}
		break;
	}
	case(Game::ShowingMenu) :
	{
		int64_t accumulator = 0;

		menu->menuState = Menu::Main;

		int64_t currTime = tickCounter.getProgramTime();

		Game::cameraManager.setCurrCamera("DefaultCamera");

		while (gameState == Game::ShowingMenu)
		{
			tickCounter.updateProgramTime();
			
			int64_t newTime = tickCounter.getProgramTime();
			int64_t frameTime = newTime - currTime;
			if (frameTime > 25000)
			{
				frameTime = 25000;
			}
			currTime = newTime;

			accumulator += frameTime;

			tickCounter.updateElapsedTime();
			tickCounter.updateTicksPerSecond();

			tickCounter.elapsedTime = dt;

			while (accumulator >= delTime.getMicroSeconds())
			{
				eventManager.handleEvents();
				menu->update(delTime);

				if (menu->menuState == Menu::BackToGame) { gameState = Playing; }

				tickCounter.updateProgramTime();
				CURR_CAM->update(delTime);

				accumulator -= delTime.getMicroSeconds();
			}

			glClearColor(back.r, back.g, back.b, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Time alpha; alpha.setSeconds(double(accumulator) / double(dt));

			Game::cameraManager.setCurrCamera(Game::map->getCurrentCamera());
			map->draw((std::sin(tickCounter.getProgramTime() * 0.00005)*0.1) + 0.5);
			Game::cameraManager.setCurrCamera("DefaultCamera");

			menu->draw(alpha.getSeconds());

			mainWindow.display();
		}
		break;
	}
	}

}

unsigned int Game::ScreenSizeX;
unsigned int Game::ScreenSizeY;
int Game::debugOptions[NO_OPTIONS];
Game::GameState Game::gameState = Uninitialized;
int Game::obDrawn = 0;
long Game::polyDrawn = 0;
sf::Window Game::mainWindow;
sf::Event Game::currentEvent;
TickCounter Game::tickCounter;
EventManager Game::eventManager;
CameraManager Game::cameraManager;
FTManager Game::fTManager;
FontManager Game::fontManager;
ShaderManager Game::shaderManager;
TextureManager Game::textureManager;
Map* Game::map;
//Player* Game::player;
std::mt19937_64 Game::rng;
DebugOverlay Game::debugOverlay;
std::string Game::MAP_NAME;
CallbackCollection Game::cb;
Time Game::delTime;
Menu* Game::menu;

std::vector<vec2f> Game::contacts;

bool Game::mapChanged = false;
std::string Game::nextMapName;
float Game::rot = 0;
bool Game::rotateD = false;
bool Game::drawUI = true;

int64_t Game::dt = 5000;
bool Game::doAlpha = true;
Colour Game::back = Colour(1.0f, 1.0f, 1.0f);

int Game::iter = 1;