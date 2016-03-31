#pragma once
#include "Shader Manager.h"

ShaderManager::ShaderManager() {}

ShaderManager::~ShaderManager() {}

void ShaderManager::init()
{
	std::cout << "Shader manager initialised" << std::endl;
	
	std::cout << "** Creating shaders **" << std::endl;
	
	createShader(std::string("min"));
	createShader(std::string("debug"));
	createShader(std::string("text"));
	createShader(std::string("minTex"));
	createShader(std::string("min2"));
	
	std::cout << "** Done creating shaders **" << std::endl;
}

Shader* ShaderManager::createShader(std::string pPath, std::string pName)
{
	if (pName == "\n")
	{
		pName = pPath;
	}

	if (shaders.find(pName) == shaders.end())
	{
		Shader* shader = new Shader(pPath);
		shaders.insert(std::make_pair(pName, shader));
		return shader;
	}
	else
	{
		std::cout << "Could not create shader: " << pName << ". SHADER " << pName << " ALREADY EXISTS." << std::endl;
		return nullptr;
	}
}

Shader* ShaderManager::createShader(std::string pPathVert, std::string pPathFrag, std::string pName)
{
	if (pName == "\n")
	{
		pName = pPathFrag + pPathVert;
	}

	if (shaders.find(pName) == shaders.end())
	{
		Shader* shader = new Shader(pPathVert, pPathFrag);
		shaders.insert(std::make_pair(pName, shader));
		return shader;
	}
	else
	{
		std::cout << "Could not create shader: " << pName << ". SHADER " << pName << " ALREADY EXISTS." << std::endl;
		return nullptr;
	}
}

Shader* ShaderManager::getShader(std::string pName)
{
	auto findShader = shaders.find(pName);
	if (findShader == shaders.end())
	{
		std::cout << "Could not get shader: " << pName << ". NOT FOUND" << std::endl;
		return nullptr;
	}
	else
	{
		return findShader->second;
	}
}

void ShaderManager::deleteShader(std::string pName)
{

}