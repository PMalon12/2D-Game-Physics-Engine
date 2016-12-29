#pragma once
#include "Include.h"
#include <fstream>

class Shader
{
	friend class ShaderManager;
private:
	Shader();
	Shader(std::string pPath);
	Shader(std::string pPathVert, std::string pPathFrag);
	~Shader();

	GLint shaderProgram;

	GLuint load(std::string pPath);
	GLuint load(std::string pPathVert, std::string pPathFrag);
	
	char name[16];

public:

	char* getName() { return name; }
	GLuint getID();
	void use();
	void stop();
};
