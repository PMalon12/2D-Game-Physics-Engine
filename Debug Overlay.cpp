#pragma once
#include "Debug Overlay.h"
#include "Text.h"
#include "Game.h"

DebugOverlay::DebugOverlay()
{
	cmd.reserve(13);
}

void DebugOverlay::init()
{
	glGenBuffers(1, &boxVBO);

	shader = Game::shaderManager.getShader("debug");

	shader->use();

	GLint colour = glGetUniformLocation(shader->getID(), "colour");
	glUniform4f(colour, 0.0f, 0.0f, 0.0f, 0.6f);

	fps = new Text();
	fpsP = new Text();

	fps->setFont(Game::fontManager.getFont("consola"));
	fps->setCharSize(255);
	fps->setColour(Colour(1.0, 1.0, 1.0, 1.0));
	fps->setPosition(vec2f(10, Game::ScreenSizeY - 500));
	fps->setScale(vec2f(1.0, 1.0));

	fpsP->setFont(Game::fontManager.getFont("consola"));
	fpsP->setCharSize(24);
	fpsP->setColour(Colour(0.2, 1.0, 0.1, 1.0));
	fpsP->setPosition(vec2f(10, Game::ScreenSizeY - 30));
	fpsP->setScale(vec2f(1.0, 1.0));

	command = new Text();

	command->setFont(Game::fontManager.getFont("consola"));
	command->setCharSize(16);
	command->setColour(Colour(0.7, 1.0, 0.7, 1.0));
	command->setPosition(vec2f(10, Game::ScreenSizeY - 20));
	command->setScale(vec2f(1.0, 1.0));

	cam = Game::cameraManager.getCamera("DefaultCamera");

	cam->setView(Camera::DefaultView);
	cam->setCenter(Game::ScreenSizeX / 2, Game::ScreenSizeY / 2);
	cam->update(Time());

	cmd.push_back(std::string("> "));
	inp.push_front(std::string("> "));

	Console::init();
}

void DebugOverlay::draw()
{


	if (Game::getOption(DRAW_DEBUG_OVERLAY))
	{
		glBindBuffer(GL_ARRAY_BUFFER, boxVBO);

		float mX = sf::Mouse::getPosition(Game::mainWindow).x;
		float mY = Game::ScreenSizeY - sf::Mouse::getPosition(Game::mainWindow).y;
		float mZ = 0;

		glm::fvec3 unp;

		unp = glm::unProject(glm::fvec3(mX, mY, mZ), Game::cameraManager.getCurrCamera()->getViewMat(), Game::cameraManager.getCurrCamera()->getProjMat(), Game::cameraManager.getCurrCamera()->getViewport());

		//fps->setString("FPS: " + std::to_string(Game::tickCounter.getTicksPerSecond()) + '\n' +
		//	"Update dt: " + std::to_string(Game::tickCounter.getElapsedTime() / 1000.f) + " ms" + '\n' +
		//	"Program time: " + std::to_string(Game::tickCounter.getProgramTime()) + '\n' +
		//	"Screen Mouse Pos: (" + std::to_string(mX) + "," + std::to_string(mY) + ')' + '\n' +
		//	"World Mouse Pos: (" + std::to_string(unp.x) + "," + std::to_string(unp.y) + ')' + '\n' +
		//	"Window Size: (" + std::to_string(Game::ScreenSizeX) + "," + std::to_string(Game::ScreenSizeY) + ')' + '\n' +
		//	"View Size: (" + std::to_string(Game::cameraManager.getCurrCamera()->getSize().x) + "," + std::to_string(Game::cameraManager.getCurrCamera()->getSize().y) + ')' + '\n' +
		//	"View Center: (" + std::to_string(Game::cameraManager.getCurrCamera()->getCenter().x) + "," + std::to_string(Game::cameraManager.getCurrCamera()->getCenter().y) + ')' + '\n' +
		//	"Objects Drawn: " + std::to_string(Game::obDrawn) + '\n' +
		//	"Polygons Drawn: " + std::to_string(Game::polyDrawn) + '\n');

		command->setString(std::string(""));

		for (auto itr = cmd.begin(); itr != cmd.end(); ++itr)
		{
			command->setString(command->getString() + (*itr) + "\n");
		}

		shader->use();

		float box[] = {
			0,0,0,
			0,Game::ScreenSizeY,0,
			550,Game::ScreenSizeY,0,
			550,0,0,
		};

		Game::cameraManager.setCurrCamera("DefaultCamera");

		GLint colour = glGetUniformLocation(shader->getID(), "colour");
		glUniform4f(colour, 0.0f, 0.0f, 0.0f, 0.6f);

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

		auto posAttrib = glGetAttribLocation(shader->getID(), "position");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(posAttrib);

		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(posAttrib);

		fps->setString("FPS: " + std::to_string(Game::tickCounter.getTicksPerSecond()) + '\n' +
			"Update dt: " + std::to_string(Game::getDT().getMilliSeconds()) + " ms" + '\n' +
			"World Mouse Pos: (" + std::to_string(unp.x) + "," + std::to_string(unp.y) + ')' + '\n' +
			"View Size: (" + std::to_string(Game::cameraManager.getCurrCamera()->getSize().x) + "," + std::to_string(Game::cameraManager.getCurrCamera()->getSize().y) + ')' + '\n' +
			"Polygons Drawn: " + std::to_string(Game::polyDrawn) + '\n');

		fps->draw();

		command->draw();

		Game::cameraManager.setCurrCamera(Game::map->getCurrentCamera());
	}
	else
	{
		if (Game::getOption(DRAW_FPS))
		{
			shader->use();

			Game::cameraManager.setCurrCamera("DefaultCamera");

			fpsP->setString(std::to_string(Game::tickCounter.getTicksPerSecond()));
			fpsP->draw();

			Game::cameraManager.setCurrCamera(Game::map->getCurrentCamera());
		}
	}
}