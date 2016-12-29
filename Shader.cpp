#pragma once
#include "Shader.h"

Shader::Shader() {}

Shader::Shader(std::string pPath) { load(pPath); }

Shader::Shader(std::string pPathVert, std::string pPathFrag) { load(pPathVert,pPathFrag); }

Shader::~Shader() { glDeleteProgram(shaderProgram); }

GLuint Shader::load(std::string pPath)
{
	if (shaderProgram != 0) //IF A SHADER ALREADY LOADED, DELETE IT
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}

	std::string vertPath = "Res/Shaders/" + pPath + ".vert";
	std::string fragPath = "Res/Shaders/" + pPath + ".frag";

	std::ifstream vertStream(vertPath, std::ifstream::in);
	std::ifstream fragStream(fragPath, std::ifstream::in);

	if (vertStream.fail())
	{
		std::cout << "Failed loading vertex shader: " << vertPath << std::endl;
		return 0;
	}

	if (fragStream.fail())
	{
		std::cout << "Failed loading fragment shader: " << fragPath << std::endl;
		return 0;
	}

	std::string fragContent;
	std::string vertContent;

	fragContent.assign(std::istreambuf_iterator<char>(fragStream), std::istreambuf_iterator<char>());
	vertContent.assign(std::istreambuf_iterator<char>(vertStream), std::istreambuf_iterator<char>());

	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* const glCharFrag = fragContent.c_str();
	const GLchar* const glCharVert = vertContent.c_str();

	glShaderSource(fragmentShader, 1, &glCharFrag, NULL);
	glShaderSource(vertexShader, 1, &glCharVert, NULL);

	auto err = glGetError();
	glCompileShader(fragmentShader);
	GLint isCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	err = glGetError();
	if (isCompiled == GL_FALSE)
	{
		std::cout << "Failed to compile fragment shader: " << pPath << std::endl;
	}

	glCompileShader(vertexShader);
	isCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		std::cout << "Failed to compile fragment shader: " << pPath << std::endl;
	}

	fragStream.close();
	vertStream.close();

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColour");
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	isCompiled = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		std::cout << "Failed to link shader: " << pPath << std::endl;

		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &errorLog[0]);

		for (int i = 0; i < errorLog.size(); ++i)
		{
			//std::cout << char(errorLog.data()[i]);
		}

		glDeleteProgram(shaderProgram);

		shaderProgram = 0;

		return 0;
	}

	std::cout << "Shader program " << pPath << " loaded and linked successfully" << std::endl;

	return shaderProgram;
}

GLuint Shader::load(std::string pPathVert, std::string pPathFrag)
{
	if (shaderProgram != 0) //IF A SHADER ALREADY LOADED, DELETE IT
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}

	std::string vertPath = "Res/Shaders/" + pPathVert + ".vert";
	std::string fragPath = "Res/Shaders/" + pPathFrag + ".frag";

	std::ifstream vertStream(vertPath, std::ifstream::in);
	std::ifstream fragStream(fragPath, std::ifstream::in);

	if (vertStream.fail())
	{
		std::cout << "Failed loading vertex shader: " << vertPath << std::endl;
		return 0;
	}

	if (fragStream.fail())
	{
		std::cout << "Failed loading fragment shader: " << fragPath << std::endl;
		return 0;
	}

	std::string fragContent;
	std::string vertContent;

	fragContent.assign(std::istreambuf_iterator<char>(fragStream), std::istreambuf_iterator<char>());
	vertContent.assign(std::istreambuf_iterator<char>(vertStream), std::istreambuf_iterator<char>());

	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* const glCharFrag = fragContent.c_str();
	const GLchar* const glCharVert = vertContent.c_str();

	glShaderSource(fragmentShader, 1, &glCharFrag, NULL);
	glShaderSource(vertexShader, 1, &glCharVert, NULL);

	glCompileShader(fragmentShader);
	GLint isCompiled = GL_FALSE;
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		std::cout << "Failed to compile fragment shader: " << pPathFrag << std::endl;
	}

	glCompileShader(vertexShader);
	isCompiled = GL_FALSE;
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		std::cout << "Failed to compile vertex shader: " << pPathVert << std::endl;
	}

	fragStream.close();
	vertStream.close();

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColour");
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	isCompiled = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		std::cout << "Failed to link shader: " << pPathVert << pPathFrag << std::endl;

		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &errorLog[0]);

		for (int i = 0; i < errorLog.size(); ++i)
		{
			//std::cout << char(errorLog.data()[i]);
		}

		glDeleteProgram(shaderProgram);

		shaderProgram = 0;

		return 0;
	}

	std::cout << "Shader program " << pPathVert << pPathFrag << " loaded and linked successfully" << std::endl;

	return shaderProgram;
}

GLuint Shader::getID() { return shaderProgram; }

void Shader::use() { glUseProgram(shaderProgram); }

void Shader::stop() { glUseProgram(0); }