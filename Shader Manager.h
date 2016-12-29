#pragma once
#include "Include.h"
#include "Shader.h"

class ShaderManager
{
	friend class Game;
public:
	ShaderManager();
	~ShaderManager();

	Shader* createShader(std::string pPath, std::string pName = "\n");
	Shader* createShader(std::string pPathVert, std::string pPathFrag, std::string pName = "\n");

	Shader* getShader(std::string pPath);
	void deleteShader(std::string pName);

private:

	void init();
	std::map<std::string, Shader*> shaders;

};