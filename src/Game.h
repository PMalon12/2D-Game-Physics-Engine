#pragma once
#include <GL\glew.h>
#include "Include.h"
#include "Tick Counter.h"
#include "Camera Manager.h"
#include "FreeType Manager.h"
#include "Font Manager.h"
#include "Debug Overlay.h"
#include "Shader Manager.h"
#include "Player.h"
#include "Map.h"
#include "Texture Manager.h"
#include "Null Entity.h"
#include "Menu.h"
#include "Event Manager.h"

#define NO_OPTIONS 13

#define DRAW_STRUCT_BOUNDS 0
#define DRAW_PATHS 1
#define DRAW_RAYS 2
#define DRAW_QUADTREE_BOUNDS 3
#define DRAW_DEBUG_OVERLAY 4
#define DRAW_CLIP_SEGMENTS 5
#define DRAW_RAY_INTERSECTS 6
#define DRAW_ENTITY_ID 7
#define DRAW_WIREFRAME 8
#define DRAW_INTERPOLATE 9
#define DRAW_CONVEX_HULL 10
#define DRAW_FPS 11
#define DRAW_ID 12

#define CURR_CAM Game::cameraManager.getCurrCamera()
#define ET Game::tickCounter.getElapsedTime()

class Game
{
	friend class Menu;
	friend class Console;
	friend class EventManager;
public:
	static void start();

	static std::vector<vec2f> contacts;

	static std::string MAP_NAME;

	static TickCounter tickCounter;
	static EventManager eventManager;
	static CameraManager cameraManager;
	static FTManager fTManager;
	static FontManager fontManager;
	static DebugOverlay debugOverlay;
	static ShaderManager shaderManager;
	static TextureManager textureManager;

	static sf::Window mainWindow;

	static std::mt19937_64 rng;

	static unsigned int ScreenSizeX;
	static unsigned int ScreenSizeY;

	static int obDrawn;
	static long polyDrawn;

	static int64_t dt;
	static Time delTime;

	static bool doAlpha;
	static bool drawUI;

	static Colour back;

	static Menu* menu;

	static int iter;

	static bool rotateD;
	static float rot;

	static Map* map;
	static CallbackCollection cb;

private:

	static bool mapChanged;
	static std::string nextMapName;

	enum GameState { Uninitialized, ShowingSplash, Paused, ShowingMenu, Playing, Exiting };
	static GameState gameState;
	
	static bool isExiting();
	static void gameLoop();

	static sf::Event currentEvent;

	static int debugOptions[NO_OPTIONS];

public:

	static NullEntity cameraEntity;

	inline static void setOption(int pOption, int pValue)
	{
		if (pOption < NO_OPTIONS)
		{
			debugOptions[pOption] = pValue;
			return;
		}
		return;
		//THROW SOME EXCEPTION
	}

	inline static int getOption(int pOption)
	{
		if (pOption < NO_OPTIONS)
		{
			return debugOptions[pOption];
		}
		return -1;
		//THROW SOME EXCEPTION
	}

	static vec2f getMousePos()
	{
		int mx = sf::Mouse::getPosition(Game::mainWindow).x;
		int my = Game::ScreenSizeY - sf::Mouse::getPosition(Game::mainWindow).y;

		return vec2f(mx, my);
	}

	static vec2f getWorldMousePos()
	{
		int mx = sf::Mouse::getPosition(Game::mainWindow).x;
		int my = Game::ScreenSizeY - sf::Mouse::getPosition(Game::mainWindow).y;

		glm::fvec3 unp;

		unp = glm::unProject(glm::fvec3(mx, my, 0), CURR_CAM->getViewMat(), CURR_CAM->getProjMat(), CURR_CAM->getViewport());

		return vec2f(unp.x, unp.y);
	}

	static void setOptionConsole(vec2d pVals)
	{
		setOption(int(pVals.x), int(pVals.y));
	}

	static Entity* getEntity(uint32_t id)
	{
		map->getEntity(id);
	}

	static void setBack(vec3d p)
	{
		back.r = p.x; back.g = p.y; back.b = p.z;
	}
	static void setDt(double p)
	{
		dt = p;
		tickCounter.elapsedTime = dt;
		delTime.setMicroSeconds(dt);
	}
	static void setVSync(double p)
	{
		mainWindow.setVerticalSyncEnabled(bool(p));
	}
	static vec3d getBack()
	{
		return vec3d(back.r, back.g, back.b);
	}
	inline static double getDt()
	{
		return delTime.getSeconds();
	}

	inline static Time getDT() { return delTime; }
	inline static void setDT(Time pdt) { delTime.setSeconds(pdt.getSeconds()); }

	static double getFPS()
	{
		return tickCounter.getTicksPerSecond();
	}

	//Map editing functions
	static void changeMap(std::string mapName)
	{
		mapChanged = true;
		nextMapName = mapName;
	}

	static void setTexClip(vec2d val)
	{
		int32_t nid(val.x);
		auto obj = map->getObject(nid);
		if (obj != nullptr)
		{
			obj->mesh->clipTextureToWorldOrigin = val.y;
		}
	}

	static void setObjCol(vec5d val)
	{
		auto obj = map->getObject(val.x);
		if (obj != nullptr)
		{
			obj->mesh->setColour(vec4d(val.y, val.z, val.w, val.v));
		}
	}

	static void removeObj(double obj)
	{
		map->removeObject(int32_t(obj));
	}

	static void setCheckpointIndex(vec2d val)
	{
		auto obj = map->getObject(val.x);
		if (obj == nullptr) { return; }
		if (obj->getID() == O_CHECKPOINT)
			map->setCheckpointIndex(reinterpret_cast<Checkpoint*>(obj), val.y);
	}

	static void setNextLevel(double id, std::string mapName)
	{
		auto obj = map->getObject(int32_t(id));
		if (obj != nullptr)
		{
			if (obj->getID() != O_END_LEVEL) { return; }
			auto el = reinterpret_cast<EndLevel*>(obj);
			el->setNextLevelName(mapName);
		}
	}

	static void setBackTex(vec2d val)
	{
		Game::map->backTexture = Game::textureManager.getTexture(val.x);
		Game::map->backTextureZoom = vec2f(val.y, val.y);
		Game::map->updateBackRect();
	}

	static void setPower(vec2d val)
	{
		auto obj = map->getObject(val.x);
		if (obj == nullptr) { return; }
		if (obj->getID() != O_POWERUP) { return; }
		auto pow = reinterpret_cast<Powerup*>(obj);
		pow->powerCode = val.y;
	}

	static void setGrav(vec2f pg)
	{
		map->properties.gravity = pg;
	}

	static void setIter(float pg)
	{
		iter = pg;
	}

	static void setRotate(float pr)
	{
		rotateD = bool(int(pr));
		rot = 0;
	}

	static void setTex(vec3d val)
	{
		int32_t nid(val.x);
		auto obj = map->getObject(nid);
		if (obj != nullptr)
		{
			auto texture = textureManager.getTexture(int32_t(val.y));
			if (texture != nullptr)
			{
				obj->setTexture(texture);
				obj->mesh->updateTextureCoords(val.z, obj->getPosition());
			}
		}
	}

	static void setGravAf(vec2d val)
	{
		int32_t nid(val.x);
		auto obj = map->getObject(nid);
		if (obj != nullptr)
		{
			obj->affectedByGravity = val.y;
		}
	}

};