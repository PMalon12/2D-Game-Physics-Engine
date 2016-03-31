#pragma once
#include "Include.h"
#include "Texture.h"
#include "Colour.h"

class RectangleShape
{
public:
	RectangleShape() {}
	~RectangleShape() { glDeleteBuffers(1, &vbo); glDeleteVertexArrays(1, &vao); }

	void init(frect r, Texture* tex);
	void updateVBO();

	void draw();
	void draw(Transform&);

	frect rectangle;

	GLuint vbo;
	GLuint vboSize;
	GLuint vao;
	Texture* texture;
	Colour c;
};