#pragma once
#include "Include.h"
#include "Text.h"
#include "Game.h"

#define FTMan Game::fTManager


#define CurrSymbol static_cast<char>(string.data()[i])

Text::Text() : font(nullptr), scale(vec2f(1, 1)), colour(1.f, 1.f, 1.f, 1.f)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	shader = Game::shaderManager.getShader("text");
	shader->use();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	posAttrib = glGetAttribLocation(shader->getID(), "position");
	colAttrib = glGetAttribLocation(shader->getID(), "colour");
	texAttrib = glGetAttribLocation(shader->getID(), "texCoord");

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);

	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(colAttrib);

	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(texAttrib);

	glBindVertexArray(0);

	modelUniform = glGetUniformLocation(shader->getID(), "model");
}

Text::~Text()
{
	if (font != nullptr) { font->releaseGlyphs(fontSize); }
}

void Text::setFont(Font* pFont)
{
	if (font == pFont){
		return;
	}
	if (font != nullptr)
	{
		font->noRefFont--;
	}
	pFont->noRefFont++;
	font = pFont;
	updateBounds();
}

void Text::setString(std::string pString)
{
	if (this->string == pString)
	{
		return;
	}
	string = pString;
	updateBounds();
}

void Text::setColour(Colour pColour)
{
	colour = pColour;
}

void Text::setPosition(vec2f pPos)
{
	pos = pPos;
}

void Text::setScale(vec2f pScale)
{
	if (scale == pScale)
	{
		return;
	}
	scale = pScale;
	updateBounds();
}

void Text::setCharSize(unsigned char pSize)
{
	if (fontSize == pSize || font == nullptr || pSize == 0)
	{
		return;
	}

	if(fontSize != 0)
		font->releaseGlyphs(fontSize);

	fontSize = pSize;
	font->loadGlyphs(fontSize);
	updateBounds();
}

void Text::draw()
{
	if ((font == nullptr) || (string.size() == 0) || (scale.x == 0) || (scale.y == 0) || (fontSize == 0))
	{
		return;
	}

	if (Game::getOption(DRAW_WIREFRAME))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	vec2f rgp(pos.x, pos.y); //RELATIVE GLYPH POSITION

	const char *p = string.c_str();

	shader->use();

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	auto cam = Game::cameraManager.getCurrCamera();

	glm::fmat4 modelMat;
	glm::translate(modelMat, glm::vec3(pos.x, pos.y, 0));

	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMat));

	while (*p != '\0')
	{
		if (*p == '\n')
		{
			rgp.x = pos.x;
			rgp.y -= fontSize;
			++p;
		}
		else
		{
			float fX = (rgp.x * scale.x) + (GetGlyphs->getPositions(*p).x * scale.x);
			float fY = (rgp.y * scale.y) + (GetGlyphs->getPositions(*p).y * scale.y);
			float w = GetGlyphs->getPixelSize(*p).x * scale.x;
			float h = GetGlyphs->getPixelSize(*p).y * scale.y;

			GLfloat vertices[] = {
				//Position			Colour									Texcoords
				fX, fY, 0,			colour.r, colour.g, colour.b, colour.a, 0.0f, 0.0f, // Bot-left
				fX + w, fY, 0,		colour.r, colour.g, colour.b, colour.a, 1.0f, 0.0f, // Bot-right
				fX, fY - h, 0,		colour.r, colour.g, colour.b, colour.a, 0.0f, 1.0f, // Top-left
				fX + w, fY - h, 0,	colour.r, colour.g, colour.b, colour.a, 1.0f, 1.0f  // Top-right
			};

			font->bindGlyph(fontSize, *p);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			rgp.x += ((font->getGlyphs(fontSize)->getAdvance(*p)).x >> 6);
			rgp.y += ((font->getGlyphs(fontSize)->getAdvance(*p)).y >> 6);

			++p;
		}
	}

	if (Game::getOption(DRAW_STRUCT_BOUNDS))
	{
		frect b = getGlobalBounds();

		auto s = Game::shaderManager.getShader("min2");
		s->use();

		auto col = glGetUniformLocation(s->getID(), "colour");
		glUniform4f(col, 1, 1, 1, 1);

		auto model = glGetUniformLocation(s->getID(), "model");
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::fmat4()));

		glBegin(GL_LINE_LOOP);

		glVertex3f(b.left, b.bot, 0);
		glVertex3f(b.left, b.bot + b.height, 0);
		glVertex3f(b.left + b.width, b.bot + b.height, 0);
		glVertex3f(b.left + b.width, b.bot, 0);

		glEnd();
	}

	glBindVertexArray(0);

	glUseProgram(0);

	if (Game::getOption(DRAW_WIREFRAME))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}