#pragma once
#include "Game.h"
#include "Entity.h"
#include "Text.h"
#include "Font.h"
#include "Player.h"

void Entity::loadFromStream(std::ifstream& ifs)
{
	//Physical::loadFromStream(ifs);
	mesh = new TriangleMesh(ifs);
}

void Entity::saveToStream(std::ofstream& ofs)
{
	Physical::saveToStream(ofs);
	mesh->saveToStream(ofs);
}