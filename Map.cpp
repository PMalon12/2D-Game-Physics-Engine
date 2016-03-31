#pragma once
#include "Map.h"
#include "Game.h"
#include <fstream>
#include <functional>
#include "GJK.h"
#include "EPA.h"
#include "Projectile.h"

void Map::update(float dt)
{
	// ADD/REMOVE STRUCTS/ENTITES ////////////////////////////////////////////////////////////////////////
	for (auto itr = entitiesToAdd.begin(); itr != entitiesToAdd.end(); ++itr)
	{
		rigids.insert(std::make_pair(*itr, *itr));
		entities.push_back(*itr);
		eQuadTree.insert(*itr);
		physicalBodies.updateBucketsForObject(*itr);
	}

	entitiesToAdd.clear();

	for (auto itr = entitiesToRemove.begin(); itr != entitiesToRemove.end(); ++itr)
	{
		physicalBodies.removeObject(*itr);
		auto i = std::find(entities.begin(), entities.end(), *itr); 
		if (i != entities.end())
		{
			entities.erase(i);
			rigids.erase(*itr);
			delete *itr;
		}
	}

	entitiesToRemove.clear();

	bool updateSQuadTree = false;

	for (auto itr = structuresToAdd.begin(); itr != structuresToAdd.end(); ++itr)
	{
		structures.push_back(*itr);
		sQuadTree.insert(*itr);
		physicalBodies.updateBucketsForObject(*itr);
		rigids.insert(std::make_pair(*itr, *itr));
		updateSQuadTree = true;
	}

	structuresToAdd.clear();

	for (auto itr = structuresToRemove.begin(); itr != structuresToRemove.end(); ++itr)
	{
		physicalBodies.removeObject(*itr);
		auto i = std::find(structures.begin(), structures.end(), *itr);
		if (i != structures.end())
		{
			structures.erase(i);
			rigids.erase(*itr);
			delete *itr;
		}
		updateSQuadTree = true;
	}

	structuresToRemove.clear();

	if (updateSQuadTree)
		sQuadTree.update(structures);

	for (auto itr = dynamicsToAdd.begin(); itr != dynamicsToAdd.end(); ++itr)
	{
		dynamics.push_back(*itr);
		dQuadTree.insert(*itr);
		rigids.insert(std::make_pair(*itr,*itr));
	}

	dynamicsToAdd.clear();

	for (auto itr = dynamicsToRemove.begin(); itr != dynamicsToRemove.end(); ++itr)
	{
		physicalBodies.removeObject(*itr);
		auto i = std::find(dynamics.begin(), dynamics.end(), *itr);
		if(i != dynamics.end())
		{
			dynamics.erase(i);
			rigids.erase(*itr);
			delete *itr;
		}
	}

	dynamicsToRemove.clear();
	// ADD/REMOVE STRUCTS/ENTITES ////////////////////////////////////////////////////////////////////////

	for (auto itr = rigids.begin(); itr != rigids.end(); ++itr)
	{
		float massFactor = itr->second->getBodyMass() * itr->second->getAffectedByGravity();
		itr->second->applyLinearForce(properties.gravity * massFactor * dt * TOSECONDS);
		(*itr).second->update(dt);
	}

	physicalBodies.manifolds.clear();

	for (auto itr = rigids.begin(); itr != rigids.end(); ++itr)
	{
		physicalBodies.updateBucketsForObject(itr->second);
		(*itr).second->updatePhysics();
	}

	physicalBodies.resolveManifolds();

	eQuadTree.update(entities);
	dQuadTree.update(dynamics);

	if (player->getHealth() <= 0)
	{
		if (checkPointNum > 0)
		{
			player->setPosition(checkPointPos);
		}
		else
		{
			player->setPosition(startPos);
		}
		player->setHealth(100);
	}

	if (editMode)
	{
		auto mouse = Game::getWorldMousePos();

		vec2f gran = placeGranularity;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			gran *= 5.f;
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			gran *= 0.5f;

		vec2f signs(mouse.x > 0 ? 1 : -1, mouse.y > 0 ? 1 : -1);
		mouse *= signs;
		mouse /= gran;
		vec2f fixed(int(ceil(mouse.x)), int(ceil(mouse.y)));
		mouse = fixed * gran;
		mouse *= signs;

		placeTransform.setTranslation(vec2f((mouse.x - (toAdd.rectangle.width * 0.5f)), (mouse.y - (toAdd.rectangle.height * 0.5f))));
		//toAdd.updateVBO();
	}
}

void Map::draw(float a)
{
	auto sh = Game::shaderManager.getShader("minTex");
	sh->use();

	auto model = glGetUniformLocation(sh->getID(), "model");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::fmat4()));

	auto col = glGetUniformLocation(sh->getID(), "colour");
	glUniform4f(col, 1.0f, 1.0f, 1.0f, 1.0f);

	glBindVertexArray(backVAO);
	glBindBuffer(GL_ARRAY_BUFFER, backVBO);
	glBindTexture(GL_TEXTURE_2D, backTexture->getID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	frect vp = Game::cameraManager.getCurrCamera()->getWorldSpaceViewport();
	
	shader->use();

	sQuadTree.queryDraw(vp, a);

	eQuadTree.queryDraw(vp, a);

	dQuadTree.queryDraw(vp, a);

	if (editMode)
	{
		placeTransform.setOrigin(vec2f(toAdd.rectangle.width / 2, toAdd.rectangle.height / 2).negate());
		toAdd.draw(placeTransform);
		Game::cameraManager.setCurrCamera("DefaultCamera");
		//editHelp.draw();
		//editModeText.draw();
		Game::cameraManager.setCurrCamera(currentCamera);
	}

	if (Game::getOption(DRAW_QUADTREE_BOUNDS) != 0)
	{
		auto s = Game::shaderManager.getShader("min");
		s->use();

		model = glGetUniformLocation(s->getID(), "model");
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::fmat4()));

		col = glGetUniformLocation(s->getID(), "colour");
		glUniform4f(col, 0.0f, 0.2f, 1.f, 0.7f);

		glUniform4f(col, 1.0f, 0.2f, 0.1f, 0.5f);

		glLineWidth(2);

		glBegin(GL_LINES);

		//sQuadTree.drawBounds();
		dQuadTree.drawBounds();
		//eQuadTree.drawBounds();
		//physicalBodies.drawBounds();

		glEnd();
	}
}

Map::Map(vec2f pSize) : sQuadTree(frect(0, 0, pSize.x, pSize.y), 0, 3, 100), dQuadTree(frect(0, 0, pSize.x, pSize.y), 0, 10, 50), eQuadTree(frect(0, 0, pSize.x, pSize.y), 0, 4, 7)
{
	properties.size.x = pSize.x;
	properties.size.y = pSize.y;

	for (int i = 0; i < 64; i+=2)
	{
		properties.mapName[i] = 'A';
		properties.mapName[i + 1] = 'B';
	}

	properties.gravity = vec2f(0, -10000);

	startPos = vec2f(200, 200);

	setCurrentCamera(Game::cameraManager.getCamera("playerCam"));
	shader = Game::shaderManager.getShader("min");

	player = new Player(); player->map = this;
	CURR_CAM->setFollowTarget(player);
	//addEntity(player);

	auto fnt = Game::fontManager.getFont("consola");
	editHelp.setFont(fnt);
	editHelp.setCharSize(18);
	editHelp.setString("Change object type: Numpad\nChange width of object: [ ]\nChange height of object: ; '\nChange object rotation: , .\nDouble granularity: LControl\nHalf granularity: LShift\nGranularity setters for console:\n   Place position: pgran\n   Scale: sgran\n   Rotation: rgran\n\nCycle through placeable\nobjects using PageUP / PageDown");
	editHelp.setPosition(vec2f(Game::ScreenSizeX - editHelp.getLocalBounds().width - 50, Game::ScreenSizeY - 50));
	editHelp.setColour(Colour(0.4, 0.4, 1.0, 1.0));

	editModeText.setFont(fnt);
	editModeText.setCharSize(18);
	editModeText.setString("Current object: STATIC STRUCTURE"); 
	editModeText.setPosition(editHelp.getGlobalBounds().botLeft - vec2f(0,30));
	editModeText.setColour(Colour(0.4, 0.4, 1.0, 1.0));

	toAdd.init(frect(0, 0, 100, 100), Game::textureManager.getTexture("d"));
	toAdd.c = Colour(0.3, 0.1, 0.8, 0.5);
	placeMode = SS;

	static const auto toggleEdit = [this](void*) -> void { 
		toggleEditMode(); 
	};
	Game::cb.addKeyPressCallback(toggleEdit, KeyEventWrap(sf::Keyboard::Dash), this, 0);

	placeGranularity = vec2f(10, 10);
	scaleGranularity = vec2f(10, 10);
	rotateGranularity = PI / 4;
	
	createMapBorder(100,10);

	//LEVEL ONE
	
	auto col = [](StaticStructure* ss, float r, float g, float b, float a) -> void { ss->mesh->r = r; ss->mesh->g = g; ss->mesh->b = b; ss->mesh->a = a; };

	//auto col2 = [](Temp* ss, float r, float g, float b, float a) -> void { ss->mesh->r = r; ss->mesh->g = g; ss->mesh->b = b; ss->mesh->a = a; };

	int uid = 0;

	auto css = [&uid, this](Triangle* sq, vec2f pos) -> StaticStructure* {auto st = new StaticStructure(new TriangleMesh(2, sq)); st->setStatic(); st->setUID(uid); st->setPosition(pos); this->addSS(st); return st; };
	auto csl = [&uid, this](Triangle* sq, vec2f pos) -> LavaStructure* {auto st = new LavaStructure(new TriangleMesh(2, sq)); st->setStatic(); st->setUID(uid); st->setPosition(pos); this->addSS(st); st->setTexture(Game::textureManager.getTexture("lava") ); return st; };

	/*
	float platformWidth = 200;

	auto sq = new Triangle[2];
	square(vec2f(5000, 100));
	auto st1 = new StaticStructure(new TriangleMesh(2, sq)); st1->setStatic(); addSS(st1); st1->setUID(1); st1->setPosition(vec2f(2600, 1600));

	square(vec2f(1000, 1000));
	auto st2 = new StaticStructure(new TriangleMesh(2, sq)); st2->setStatic(); addSS(st2); st2->setUID(2); st2->setPosition(vec2f(2000, 600));

	square(vec2f(50, 1400));
	//auto st3 = new StaticStructure(new TriangleMesh(2, sq)); st3->setStatic(); addSS(st3); st3->setUID(3); st3->setPosition(vec2f(4000, 800));
	auto st3 = new Temp(); auto nn = new TriangleMesh(2, sq); st3->setMesh(nn); addEntity(st3); st3->setPosition(vec2f(4000, 800)); st3->setBodyMass(1000); st3->setBodyInertiaMoment(100009900);

	square(vec2f(50, 50));
	auto st4 = new StaticStructure(new TriangleMesh(2, sq)); st4->setStatic(); addSS(st4); st4->setUID(4); st4->setPosition(vec2f(4050, 125));

	square(vec2f(platformWidth, 50));
	auto st5 = new StaticStructure(new TriangleMesh(2, sq)); st5->setStatic(); addSS(st5); st5->setUID(5); st5->setPosition(vec2f(6000, 500));

	square(vec2f(platformWidth, 50));
	auto st6 = new StaticStructure(new TriangleMesh(2, sq)); st6->setStatic(); addSS(st6); st6->setUID(6); st6->setPosition(vec2f(7000, 1000));

	square(vec2f(platformWidth, 50));
	auto st7 = new StaticStructure(new TriangleMesh(2, sq)); st7->setStatic(); addSS(st7); st7->setUID(7); st7->setPosition(vec2f(8000, 1500));

	square(vec2f(platformWidth, 50));
	auto st8 = new StaticStructure(new TriangleMesh(2, sq)); st8->setStatic(); addSS(st8); st8->setUID(8); st8->setPosition(vec2f(9000, 2000));

	square(vec2f(platformWidth, 50));
	auto st9 = new StaticStructure(new TriangleMesh(2, sq)); st9->setStatic(); addSS(st9); st9->setUID(9); st9->setPosition(vec2f(8000, 2500));

	square(vec2f(platformWidth, 50));
	auto st10 = new StaticStructure(new TriangleMesh(2, sq)); st10->setStatic(); addSS(st10); st10->setUID(10); st10->setPosition(vec2f(7000, 3000));

	square(vec2f(100, 2000));
	auto st11 = new StaticStructure(new TriangleMesh(2, sq)); st11->setStatic(); addSS(st11); st11->setUID(11); st11->setPosition(vec2f(5050, 2650));

	square(vec2f(6100, 100));
	auto st12 = new StaticStructure(new TriangleMesh(2, sq)); st12->setStatic(); addSS(st12); st12->setUID(12); st12->setPosition(vec2f(3150, 3700));

	square(vec2f(4900, 2000));
	auto st13 = new StaticStructure(new TriangleMesh(2, sq)); st13->setStatic(); addSS(st13); st13->setUID(13); st13->setPosition(vec2f(2550, 2650)); col(st13, 0.9f, 0.9f, 0.7f, 1.0);

	std::cout << std::endl;

	for (int i = 0; i < 7; ++i)
	{
		square(vec2f(200, 210));
		float r, g, b;
		if (i % 2 == 0)
		{
			r = 0.75; g = 0.25; b = 0.4;
		}
		else
		{
			r = 0.4; g = 0.4; b = 0.75;
		}
		//auto st14 = new StaticStructure(new TriangleMesh(2, sq)); st14->setStatic(); addSS(st14); st14->setUID(14+i); st14->setPosition(vec2f(4000, 3800 + (i*100))); col(st14, r, g, b, 1.0);
		auto st14 = new Temp(); auto nm = new TriangleMesh(2, sq); st14->setMesh(nm); addEntity(st14); st14->setPosition(vec2f(4000, 3950 + (i*210))); st14->setBodyMass(100); st14->setBodyInertiaMoment(3000000); col2(st14, r, g, b, 1.0);
		st14->staticFriction = 0.3f;
		st14->dynamicFriction = 0.3f;
	}

	square(vec2f(100, 200));
	auto st14 = new StaticStructure(new TriangleMesh(2, sq)); st14->setStatic(); addSS(st14); st14->setUID(30); st14->setPosition(vec2f(5500, 200)); col(st14, 1.f, 1.f, 1.f, 1.0);
	
	squaretex(vec2f(4350, 75), vec2f(128, 128));
	auto st15 = new LavaStructure(new TriangleMesh(2, sq)); st15->setStatic(); addSS(st15); st15->setUID(31); st15->setPosition(vec2f(7725, 137.5)); col(st15, 1.f, 1.f, 1.f, 1.0); st15->setTexture(Game::textureManager.getTexture("lava"));
	l = st15; l->setID(O_SS_LAVA); l->staticFriction = 1; l->dynamicFriction = 1; l->pushBackStrength = 0.0;

	square(vec2f(9800, 100));
	auto st16 = new StaticStructure(new TriangleMesh(2, sq)); st16->setStatic(); addSS(st16); st16->setUID(32); st16->setPosition(vec2f(5000, 5300)); col(st16, 0.8f, 0.1f, 0.1f, 1.0);

	square(vec2f(9800, 4550));
	auto st17 = new StaticStructure(new TriangleMesh(2, sq)); st17->setStatic(); addSS(st17); st17->setUID(33); st17->setPosition(vec2f(5000, 7625));

	square(vec2f(50, 16));
	auto st18 = new StaticStructure(new TriangleMesh(2, sq)); st18->setStatic(); addSS(st18); st18->setUID(34); st18->setPosition(vec2f(500, 800)); col(st18, 0.9f, 0.9f, 0.9f, 0.8);

	square(vec2f(150, 400))
	//auto st19 = new StaticStructure(new TriangleMesh(2, sq)); st19->setStatic(); addSS(st19); st19->setUID(35); st19->setPosition(vec2f(500, 958)); col(st19, 0.95f, 0.3f, 0.2f, 1.f); 
	auto st19 = new Temp(); auto n = new TriangleMesh(2, sq); st19->setMesh(n); addEntity(st19); st19->setPosition(vec2f(500, 1008)); st19->setBodyMass(1000); st19->setBodyInertiaMoment(FLT_MAX);

	square(vec2f(200, 1000));
	auto st20 = new StaticStructure(new TriangleMesh(2, sq)); st20->setStatic(); addSS(st20); st20->setUID(36); st20->setPosition(vec2f(900, 1050));

	for (int i = 0; i < 7; ++i)
	{
		square(vec2f(200, 210));
		float r, g, b;
		if (i % 2 != 0)
		{
			r = 0.75; g = 0.25; b = 0.4;
		}
		else
		{
			r = 0.4; g = 0.4; b = 0.75;
		}
		//auto st14 = new StaticStructure(new TriangleMesh(2, sq)); st14->setStatic(); addSS(st14); st14->setUID(37 + i); st14->setPosition(vec2f(2000, 3800 + (i * 100))); col(st14, r, g, b, 1.0);
		auto st14 = new Temp(); auto nm = new TriangleMesh(2, sq); st14->setMesh(nm); addEntity(st14); st14->setPosition(vec2f(2000, 3950 + (i*210))); st14->setBodyMass(100); st14->setBodyInertiaMoment(3000000); col2(st14, r, g, b, 1.0);
		st14->staticFriction = 0.4f;
		st14->dynamicFriction = 0.3f;
	}

	static auto chk1 = [this](Map* map) -> void { map->player->setPosition(vec2f(200, 200)); };
	static auto chk1Active = [this](Map* map) -> void { std::cout << "CHECKPOINT 1 ACTIVE" << std::endl; };
	static auto chk2 = [this](Map* map) -> void { map->player->setPosition(vec2f(4000, 1000)); };
	static auto chk2Active = [this](Map* map) -> void {std::cout << "CHECKPOINT 2 ACTIVE" << std::endl; };

	checkpoints.push_back(Checkpoint(frect(100, 100, 1400, 400), this, chk1, chk1Active, true));
	checkpoints.push_back(Checkpoint(frect(1500, 1100, 1000, 450), this, chk2, chk2Active, true));

	static auto tut1 = [this](Map* map) -> void { std::cout << "TUTORIAL" << std::endl; };

	actionAreas.push_back(ActionArea(frect(4000, 100, 1000, 1000), this, tut1, false));

	*/

	auto sq = new Triangle[2];

	square(vec2f(100, 100));

	// auto en = new EndLevel(new TriangleMesh(2, sq)); en->setPosition(vec2f(1000, 200)); en->setStatic(); addStatic(static_cast<StaticStructure*>(en));

	GLuint modelUniform = glGetUniformLocation(shader->getID(), "model");
	GLuint corrUniform = glGetUniformLocation(shader->getID(), "renderCorrection");

	backTexture = Game::textureManager.getTexture("backTexture");
	backTextureZoom = vec2f(0.2,0.2);

	glGenBuffers(1, &backVBO);
	updateBackRect();
}

Map::Map(std::string pMapName)
{
	//sQuadTree(frect(0,0,properties.mapSize.x,properties.mapSize.y),0)
	loadMap(pMapName);
}

Map::~Map()
{
	clear();
	for (auto structuresItr = structures.begin(); structuresItr != structures.end(); ++structuresItr)
	{
		delete *structuresItr;
	}
}

void Map::loadMap(std::string pMapName)
{
	clear();

	std::ifstream mapFile;
	mapFile.open(std::string("map/") + pMapName, std::ios::binary);
	mapFile.read(properties.mapName, sizeof(properties.mapName));
	mapFile.read((char*)(&properties.size), sizeof(vec2f));
	mapFile.read((char*)(&properties.gravity), sizeof(vec2f));
	uint32_t tid;
	mapFile.read((char*)&tid, sizeof(tid)); 
	backTexture = Game::textureManager.getTexture(tid);
	mapFile.read((char*)&backTextureZoom, sizeof(backTextureZoom));
	updateBackRect();

	int numStruct;
	mapFile.read((char*)(&numStruct), sizeof(numStruct));
	structures.reserve(numStruct);

	for (int j = 0; j < numStruct; ++j)
	{
		uint32_t entID;
		mapFile.read((char*)&entID, sizeof(entID));
		switch (entID)
		{
		case(O_STATIC_STRUCT): {
			auto ss = new StaticStructure(mapFile);
			addStatic(ss);
			ss->setID(entID);
			break; }
		case(O_CHECKPOINT) : {
			auto ss = new Checkpoint(mapFile);
			addStatic(ss);
			ss->setID(entID);
			break; }
		case(O_SS_LAVA) : {
			auto ls = new LavaStructure(mapFile);
			addStatic(ls);
			ls->setID(entID);
			break; }
		case(O_END_LEVEL) : {
			auto el = new EndLevel(mapFile);
			addStatic(el);
			el->setID(entID);
			break; }
		case(O_POWERUP) : {
			auto el = new Powerup(mapFile);
			addStatic(el);
			el->setID(entID);
			break; }
		default:
			printf("Unrecognised object ID: %#08x\n", entID);
			//THROW CANNOT LOAD
		}
	}

	mapFile.read((char*)(&numStruct), sizeof(numStruct));
	dynamics.reserve(numStruct);

	for (int j = 0; j < numStruct; ++j)
	{
		uint32_t entID;
		mapFile.read((char*)&entID, sizeof(entID));
		switch (entID)
		{
		case(O_DYNAMIC_STRUCT) : {
			auto ds = new DynamicStructure(mapFile);
			addDynamic(ds); ds->staticFriction = 0.25f; ds->dynamicFriction = 0.2f;
			ds->setID(entID);
			break; }
		default:
			printf("Unrecognised entity ID: %#08x\n", entID);
			//THROW CANNOT LOAD
		}
	}

	int numEnt;
	mapFile.read((char*)(&numEnt), sizeof(numEnt));

	for (int i = 0; i < numEnt; ++i)
	{
		uint32_t entID;
		mapFile.read((char*)&entID,sizeof(entID));
		switch (entID)
		{
		case(O_PLAYER) :{
			player = new Player(mapFile);
			entities.push_back(player);
			break; }
		case(O_PROJECTILE) :{
			Projectile* n = new Projectile(mapFile);
			entities.push_back(n);
			break; }
		}
		entities.back()->setID(entID);
		entities.back()->init();
		rigids.insert(std::make_pair(entities.back(), entities.back()));
	}

	std::cout << "Loaded map: " << pMapName << " : " << mapFile.tellg() << " bytes" << std::endl;

	mapFile.close();
}

void Map::saveMap(std::string pMapName)
{
	mapPath = pMapName;

	std::ofstream mofs;
	mofs.open(std::string("map/") + pMapName, std::ios::binary);
	mofs.write(properties.mapName, sizeof(properties.mapName));
	mofs.write((char*)(&properties.size), sizeof(vec2f));
	mofs.write((char*)(&properties.gravity), sizeof(vec2f));
	auto tid = backTexture->getTextureLocID();
	mofs.write((char*)(&tid), sizeof(tid));
	mofs.write((char*)&backTextureZoom, sizeof(backTextureZoom));

	int numStruct = structures.size();
	mofs.write((char*)(&numStruct), sizeof(numStruct));

	for (auto itr = structures.begin(); itr != structures.end(); ++itr)
	{
		auto id = (*itr)->getID();
		mofs.write((char*)&id, sizeof(id));
		(*itr)->saveToStream(mofs);
	}

	int numDynamic = dynamics.size();
	mofs.write((char*)(&numDynamic), sizeof(numDynamic));

	for (auto itr = dynamics.begin(); itr != dynamics.end(); ++itr)
	{
		auto id = (*itr)->getID();
		mofs.write((char*)&id, sizeof(id));
		(*itr)->saveToStream(mofs);
	}

	int numEnt = entities.size();
	mofs.write((char*)(&numEnt), sizeof(numEnt));

	for (auto itr = entities.begin(); itr != entities.end(); ++itr)
	{
		auto id = (*itr)->getID();
		mofs.write((char*)&id, sizeof(id));
		(*itr)->saveToStream(mofs);
	}

	std::cout << "Map saved: " << pMapName << " : " << mofs.tellp() << " bytes" << std::endl;

	mofs.close();
}

void Map::toggleEditMode()
{
	editMode = editMode == Play ? Edit : Play;

	static const auto addStruct = [this](void*) -> void
	{
		Transform tr; tr.rotate(placeTransform.getRotation());
		auto mouse = Game::getWorldMousePos();
		auto sq = new Triangle[2];
		switch (placeMode)
		{
		case(SS) : {
			auto css = [this, &tr](Triangle* sq, vec2f pos) -> StaticStructure* {auto st = new StaticStructure(new TriangleMesh(2, sq, tr)); st->setStatic();  st->setUID(structures.size() + dynamics.size() + entities.size() + 1); st->setPosition(pos); this->addStatic(st); return st; };
			square(vec2f(toAdd.rectangle.width, toAdd.rectangle.height));
			addHistory.push_back(css(sq, placeTransform.getTranslation() + (toAdd.rectangle.size * 0.5)));
			std::cout << "ADDED SS " << addHistory.size() << std::endl;
			break; }
		case(DS) : {
			auto cds = [this, &tr](Triangle* sq, vec2f pos) -> DynamicStructure* {auto st = new DynamicStructure(new TriangleMesh(2, sq, tr)); st->calculateMassAndInertia(0.01); st->setUID(structures.size() + dynamics.size() + entities.size() + 1); st->setPosition(pos); this->addDynamic(st); return st; };
			square(vec2f(toAdd.rectangle.width, toAdd.rectangle.height));
			addHistory.push_back(cds(sq, placeTransform.getTranslation() + (toAdd.rectangle.size * 0.5)));
			addHistory.back()->affectedByGravity = true;
			std::cout << "ADDED DS " << addHistory.size() << std::endl;
			break; }
		case(Lava) : {
			auto cds = [this, &tr](Triangle* sq, vec2f pos) -> LavaStructure* {auto st = new LavaStructure(new TriangleMesh(2, sq, tr)); st->setStatic();  st->setUID(structures.size() + dynamics.size() + entities.size() + 1); this->addStatic(st); st->setPosition(pos); st->setTexture(Game::textureManager.getTexture("lava")); return st; };
			squaretex(vec2f(toAdd.rectangle.width, toAdd.rectangle.height), (vec2f(Game::textureManager.getTexture("lava")->getWidth(), Game::textureManager.getTexture("lava")->getHeight()) * 8));
			addHistory.push_back(cds(sq, placeTransform.getTranslation() + (toAdd.rectangle.size * 0.5)));
			std::cout << "ADDED Lava " << addHistory.size() << std::endl;
			break; }
		case(CHKPoint) : {
			auto cds = [this, &tr](Triangle* sq, vec2f pos) -> Checkpoint* {auto st = new Checkpoint(new TriangleMesh(2, sq, tr)); st->setStatic();   st->setUID(structures.size() + dynamics.size() + entities.size() + 1); this->addStatic(st); st->setPosition(pos); st->setTexture(Game::textureManager.getTexture("d")); return st; };
			square(vec2f(toAdd.rectangle.width, toAdd.rectangle.height));
			addHistory.push_back(cds(sq, placeTransform.getTranslation() + (toAdd.rectangle.size * 0.5)));
			std::cout << "ADDED Chekpoint " << addHistory.size() << std::endl;
			break; }
		case(EndLvl) : {
			auto cds = [this, &tr](Triangle* sq, vec2f pos) -> EndLevel* {auto st = new EndLevel(new TriangleMesh(2, sq, tr)); st->setStatic();   st->setUID(structures.size() + dynamics.size() + entities.size() + 1); this->addStatic(st); st->setPosition(pos); st->setTexture(Game::textureManager.getTexture("d")); return st; };
			square(vec2f(toAdd.rectangle.width, toAdd.rectangle.height));
			addHistory.push_back(cds(sq, placeTransform.getTranslation() + (toAdd.rectangle.size * 0.5)));
			std::cout << "ADDED End Level " << addHistory.size() << std::endl;
			break; }
		case(PWRup) : {
			auto cds = [this, &tr](vec2f pos) -> Powerup* {auto st = new Powerup(); st->setStatic();   st->setUID(structures.size() + dynamics.size() + entities.size() + 1); this->addStatic(st); st->setPosition(pos); st->setTexture(Game::textureManager.getTexture("d")); return st; };
			square(vec2f(toAdd.rectangle.width, toAdd.rectangle.height));
			auto n = cds(placeTransform.getTranslation() + (toAdd.rectangle.size * 0.5));
			addHistory.push_back(n); n->powerCode = 0;
			n->setTexture(Game::textureManager.getTexture("push"));
			std::cout << "ADDED Powerup " << addHistory.size() << std::endl;
			break; }
		}
	};

	static const auto getGran = [this](vec2f g) -> vec2f
	{
		vec2f ret = g;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			ret *= 5.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			ret *= 0.5f;
		}
		return ret;
	};

	static const auto getGran2 = [this](float g) -> float
	{
		float ret = g;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			ret *= 5.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			ret *= 0.5f;
		}
		return ret;
	};

	static const auto addXSize = [&](void*) -> void
	{
		static float timeSinceLast = 0;
		timeSinceLast += Game::getDT().getSeconds();
		if (timeSinceLast > 0.1f)
		{
			timeSinceLast = 0;
			toAdd.rectangle.width += getGran(scaleGranularity).x; toAdd.updateVBO();
		}
	};

	static const auto subXSize = [&](void*) -> void
	{
		static float timeSinceLast = 0;
		timeSinceLast += Game::getDT().getSeconds();
		if (timeSinceLast > 0.1f)
		{
			timeSinceLast = 0;
			toAdd.rectangle.width -= getGran(scaleGranularity).x; toAdd.updateVBO();
		}
	};

	static const auto addYSize = [&](void*) -> void
	{
		static float timeSinceLast = 0;
		timeSinceLast += Game::getDT().getSeconds();
		if (timeSinceLast > 0.1f)
		{
			timeSinceLast = 0;
			toAdd.rectangle.height += getGran(scaleGranularity).y; toAdd.updateVBO();
		}
	};

	static const auto subYSize = [&](void*) -> void
	{
		static float timeSinceLast = 0;
		timeSinceLast += Game::getDT().getSeconds();
		if (timeSinceLast > 0.1f)
		{
			timeSinceLast = 0;
			toAdd.rectangle.height -= getGran(scaleGranularity).y; toAdd.updateVBO();
		}
	};

	static const auto moveCamL = [this](void*) -> void
	{
		CURR_CAM->getFollowTarget()->move(vec2f(-1000, 0) * Game::getDT().getSeconds());
	};

	static const auto moveCamR = [this](void*) -> void
	{
		CURR_CAM->getFollowTarget()->move(vec2f(1000, 0) * Game::getDT().getSeconds());
	};

	static const auto moveCamU = [this](void*) -> void
	{
		CURR_CAM->getFollowTarget()->move(vec2f(0, 1000) * Game::getDT().getSeconds());
	};

	static const auto moveCamD = [this](void*) -> void
	{
		CURR_CAM->getFollowTarget()->move(vec2f(0, -1000) * Game::getDT().getSeconds());
	};

	static const auto setSS = [this](void*) -> void
	{
		placeMode = SS;
		toAdd.c = Colour(0.3, 0.1, 0.8, 0.5);
	};

	static const auto setDS = [this](void*) -> void
	{
		placeMode = DS;
		toAdd.c = Colour(1.0, 0.7, 0.3, 0.5);
	};

	static const auto setLS = [this](void*) -> void
	{
		placeMode = Lava;
		toAdd.c = Colour(1.0, 0.2, 0.2, 0.5);
	};

	static const auto setCHK = [this](void*) -> void
	{
		placeMode = CHKPoint;
		toAdd.c = Colour(0.3, 0.8, 0.8, 0.5);
	};

	static const auto setEND = [this](void*) -> void
	{
		placeMode = EndLvl;
		toAdd.c = Colour(0.3, 0.8, 0.3, 0.5);
	};

	static const auto setPWR = [this](void*) -> void
	{
		placeMode = PWRup;
		toAdd.c = Colour(0.8, 0.3, 0.8, 0.5);
	};

	static const auto addAng = [&](void*) -> void
	{
		static float timeSinceLast = 0;
		timeSinceLast += Game::getDT().getSeconds();
		if (timeSinceLast > 0.1f)
		{
			timeSinceLast = 0;
			placeTransform.rotate(getGran2(rotateGranularity));
		}
	};

	static const auto subAng = [&](void*) -> void
	{
		static float timeSinceLast = 0;
		timeSinceLast += Game::getDT().getSeconds();
		if (timeSinceLast > 0.1f)
		{
			timeSinceLast = 0;
			placeTransform.rotate(-getGran2(rotateGranularity));
		}
	};

	static const auto undo = [this](void*) -> void
	{
		if (addHistory.size() != 0)
		{
			auto last = addHistory.back();
			if (last->getID() == O_STATIC_STRUCT || last->getID() == O_SS_LAVA || last->getID() == O_CHECKPOINT || last->getID() == O_END_LEVEL)
			{
				removeStructure(static_cast<StaticStructure*>(last)); addHistory.pop_back();
			}
			else if (last->getID() == O_DYNAMIC_STRUCT)
			{
				removeDynamic(static_cast<DynamicStructure*>(last)); addHistory.pop_back();
			}
			
			std::cout << "REMOVED " << addHistory.size() << std::endl;
		}
	};

	if (editMode)
	{
		Game::cameraManager.setCurrCamera("editCam");
		setCurrentCamera(CURR_CAM);
		player->deregisterInputCallbacks();

		Game::cb.addKeyStateCallback(moveCamU, KeyEventWrap(sf::Keyboard::W), this, 0);
		Game::cb.addKeyStateCallback(moveCamD, KeyEventWrap(sf::Keyboard::S), this, 0);
		Game::cb.addKeyStateCallback(moveCamL, KeyEventWrap(sf::Keyboard::A), this, 0);
		Game::cb.addKeyStateCallback(moveCamR, KeyEventWrap(sf::Keyboard::D), this, 0);

		Game::cb.addMousePressCallback(addStruct, MouseEventWrap(sf::Mouse::Left), this, 0);
		Game::cb.addKeyStateCallback(addXSize, KeyEventWrap(sf::Keyboard::RBracket), this, 0);
		Game::cb.addKeyStateCallback(subXSize, KeyEventWrap(sf::Keyboard::LBracket), this, 0);
		Game::cb.addKeyStateCallback(addYSize, KeyEventWrap(sf::Keyboard::Quote), this, 0);
		Game::cb.addKeyStateCallback(subYSize, KeyEventWrap(sf::Keyboard::SemiColon), this, 0);
		Game::cb.addKeyPressCallback(undo, KeyEventWrap(sf::Keyboard::X), this, 0);

		Game::cb.addKeyPressCallback(setSS, KeyEventWrap(sf::Keyboard::Numpad1), this, 0);
		Game::cb.addKeyPressCallback(setDS, KeyEventWrap(sf::Keyboard::Numpad2), this, 0);
		Game::cb.addKeyPressCallback(setLS, KeyEventWrap(sf::Keyboard::Numpad3), this, 0);
		Game::cb.addKeyPressCallback(setCHK, KeyEventWrap(sf::Keyboard::Numpad4), this, 0);
		Game::cb.addKeyPressCallback(setEND, KeyEventWrap(sf::Keyboard::Numpad5), this, 0);
		Game::cb.addKeyPressCallback(setPWR, KeyEventWrap(sf::Keyboard::Numpad6), this, 0);

		Game::cb.addKeyStateCallback(addAng, KeyEventWrap(sf::Keyboard::Comma), this, 0);
		Game::cb.addKeyStateCallback(subAng, KeyEventWrap(sf::Keyboard::Period), this, 0);
	}
	else
	{
		Game::cameraManager.setCurrCamera("playerCam");
		setCurrentCamera(CURR_CAM);

		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::W), this, 0);
		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::S), this, 0);
		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::A), this, 0);
		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::D), this, 0);

		player->registerInputCallbacks();
		Game::cb.removeMousePressCallback(MouseEventWrap(sf::Mouse::Left), this, 0);
		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::RBracket), this, 0);
		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::LBracket), this, 0);
		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::SemiColon), this, 0);
		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::Quote), this, 0);
		Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::X), this, 0);

		Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Numpad1), this, 0);
		Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Numpad2), this, 0);
		Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Numpad3), this, 0);
		Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Numpad4), this, 0);
		Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Numpad5), this, 0);
		Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Numpad6), this, 0);

		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::Comma), this, 0);
		Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::Period), this, 0);
	}
}

void Map::updateBackRect()
{
	glBindBuffer(GL_ARRAY_BUFFER, backVBO);

	GLfloat vertices[] = { 0.f, 0.f,								-10.f, 0.f, 0.f,
		0.f, properties.size.y,					-10.f, 0.f, properties.size.y * backTextureZoom.y / backTexture->getHeight(),
		properties.size.x, properties.size.y,	-10.f, properties.size.x * backTextureZoom.x / backTexture->getWidth(), properties.size.y * backTextureZoom.y / backTexture->getHeight(),
		properties.size.x, 0.f,					-10.f, properties.size.x * backTextureZoom.x / backTexture->getWidth(), 0.f };

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	shader = Game::shaderManager.getShader("minTex");

	glGenVertexArrays(1, &backVAO);
	glBindVertexArray(backVAO);

	auto posAttrib = glGetAttribLocation(shader->getID(), "position");
	auto texAttrib = glGetAttribLocation(shader->getID(), "texCoord");

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);

	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(texAttrib);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}