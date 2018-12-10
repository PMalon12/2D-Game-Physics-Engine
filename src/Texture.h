#pragma once
#include "Include.h"
#include "SOIL.h"

class Texture
{
public:
	Texture() { glGenTextures(1, &textureID); }
	Texture(std::string name, int32_t pid = -1) { glGenTextures(1, &textureID); loadTexture(name); id = pid; }
	~Texture() { glDeleteTextures(1, &textureID); }

	void loadTexture(std::string fileName)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	void loadFromMem(float* source, size_t width, size_t height)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, source);
	}

	GLuint getID() { return textureID; }
	uint32_t getTextureLocID() { return id; }
	const char* getName() const { return name; }
	void setName(const char a[16]) { strncpy_s(name, a, 16); }

	inline int getHeight() const { return height; }
	inline int getWidth() const { return width; }
	inline vec2f getSize() const { return vec2f(width, height); }

	int32_t id;

private:

	int width, height;
	GLuint textureID;
	uint32_t textureLocID;
	char name[16];
};