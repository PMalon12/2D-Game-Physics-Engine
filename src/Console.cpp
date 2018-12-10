#pragma once
#include "Console.h"
#include "Game.h"
#include "Vector.h"
#include <istream>

//#define SetFunc1Var(func) ConsoleSetFunc(new std::function<void(double)>(func))
//#define SetFunc2Var(func) ConsoleSetFunc(new std::function<void(vec2d)>(func))
//#define SetFunc3Var(func) ConsoleSetFunc(new std::function<void(vec3d)>(func))
//#define SetFunc4Var(func) ConsoleSetFunc(new std::function<void(vec4d)>(func))
//
//#define GetFunc1Var(func) ConsoleGetFunc(new std::function<double(void)>(func))
//#define GetFunc2Var(func) ConsoleGetFunc(new std::function<vec2d(void)>(func))
//#define GetFunc3Var(func) ConsoleGetFunc(new std::function<vec3d(void)>(func))
//#define GetFunc4Var(func) ConsoleGetFunc(new std::function<vec4d(void)>(func))

//#define SetFunc1Var(func) ConsoleSetFunc<double>(new std::function<void(double)>(func))
//#define SetFunc2Var(func) ConsoleSetFunc<double>(new std::function<void(vec2d)>(func))
//#define SetFunc3Var(func) ConsoleSetFunc<double>(new std::function<void(vec3d)>(func))
//#define SetFunc4Var(func) ConsoleSetFunc<double>(new std::function<void(vec4d)>(func))

Console::Console()
{

}

void Console::init()
{
	addSetter("back", new SetFunc3Var(Game::setBack));
	addGetter("back", new GetFunc3Var(Game::getBack));

	addSetter("dt", new SetFunc1Var(Game::setDt));
	addGetter("dt", new GetFunc1Var(Game::getDt));

	addSetter("vsync", new SetFunc1Var(Game::setVSync));

	addGetter("fps", new GetFunc1Var(Game::getFPS));
}

std::string Console::interpretCommand(std::string cmd)
{
	std::string action;
	std::string id;
	std::string params;
	

	std::istringstream input(cmd);

	try {
		std::getline(input, action, ' ');

		if (action == "set" || action == "Set" || action == "SET")
		{
			std::getline(input, id, ' ');
			std::getline(input, params);
			return set(id, params);
		}
		else if (action == "get" || action == "Get" || action == "GET")
		{
			std::getline(input, id, ' ');
			return get(id);
		}
		else if (action == "load" || action == "Load" || action == "LOAD")
		{
			std::getline(input, id, '\n');
			Game::map->loadMap(id);
			return "Loaded Map : " + id;
		}
		else if (action == "save" || action == "Save" || action == "SAVE")
		{
			std::getline(input, id, '\n');
			Game::map->saveMap(id);
			return "Saved Map : " + id;
		}
		else if (action == "next" || action == "Next" || action == "NEXT")
		{
			std::getline(input, id, ',');
			std::getline(input, params, '\n');
			Game::setNextLevel(std::stod(id), params);
			return "Next map set to " + params;
		}
		else if (action == "help" || action == "Help" || action == "HELP")
		{
			return "set [id] [val]\nget [id]\nsave [map name]\nload [map name]\n next [id] [map name]";
		}
	}
	catch (ParamCountMismatchEx ex)
	{
		return ex.message;
	}
	catch (IdNotFoundEx ex)
	{
		return ex.message;
	}
	catch (...)
	{
		return "ERROR INTERPRETING COMMAND";
	}

	return std::string("Command not recognised: ") + action;

}

std::string Console::set(std::string id, std::string params)
{
	std::vector<std::string> idTokens;
	auto nextStartIds = id.begin();
	for (auto itr = id.begin(); itr != id.end(); ++itr)
	{
		if (*itr == '.')
		{
			idTokens.push_back(std::string(nextStartIds, itr));
			nextStartIds = itr + 1;
		}
		if (itr == (id.end() - 1))
		{
			idTokens.push_back(std::string(nextStartIds, id.end()));
			nextStartIds = itr + 1;
		}
	}

	double param[32];

	char numParams = 1;
	for (auto itr = params.begin(); itr != params.end(); ++itr)
	{
		if (*itr == ' ')
		{
			itr = params.erase(itr);
		}
		if (*itr == ',')
		{
			++numParams;
		}
	}

	char count = 0;
	auto nextStart = params.begin();
	for (auto itr = params.begin(); itr != params.end(); ++itr)
	{
		if (*itr == ',')
		{
			param[count] = std::stod(std::string(nextStart, itr));
			nextStart = itr + 1;
			++count;
		}
		if (itr == (params.end() - 1))
		{
			param[count] = std::stod(std::string(nextStart, params.end()));
			nextStart = itr + 1;
			++count;
		}
	}

	ConsoleSetFunc* func;
	if (idTokens.size() == 1)
	{
		auto find = setters.find(id);
		if(find != setters.end())
			func = find->second;
	}
	else
	{
		if (idTokens.front() == "ent")
		{

		} else if (idTokens.front() == "map")
		{

		} else if (idTokens.front() == "ent")
		{

		}
	}

	auto find = setters.find(id);
	if (find != setters.end())
	{
		if (find->second->getNVar() != numParams)
		{
			throw ParamCountMismatchEx(id, find->second->getNVar(), numParams);
		}

		switch (numParams)
		{
		case 1:
			find->second->operator()(param[0]);
			return id + " SET to " + params;
			break;
		case 2:
			find->second->operator()(param[0], param[1]);
			return id + " SET to " + params;
			break;
		case 3:
			find->second->operator()(param[0], param[1], param[2]);
			return id + " SET to " + params;
			break;
		case 4:
			find->second->operator()(param[0], param[1], param[2], param[3]);
			return id + " SET to " + params;
			break;
		case 5:
			find->second->operator()(param[0], param[1], param[2], param[3], param[4]);
			return id + " SET to " + params;
			break;
		}
	}
	else
	{
		throw IdNotFoundEx(id);
	}
}

std::string Console::get(std::string id)
{
	auto find = getters.find(id);
	if (find != getters.end())
	{
		return id + " is " + find->second->operator()();
	}
	else
	{
		throw IdNotFoundEx(id);
	}
}

std::map<std::string, ConsoleSetFunc*> Console::setters;
std::map<std::string, ConsoleGetFunc*> Console::getters;