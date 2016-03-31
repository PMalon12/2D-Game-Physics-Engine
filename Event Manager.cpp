#pragma once
#include "Event Manager.h"
#include "Game.h"
#include "Include.h"

void EventManager::handleEvents()
{
	while (Game::mainWindow.pollEvent(currentEvent))
	{
		switch (currentEvent.type)
		{
			case(sf::Event::EventType::KeyPressed) :
			{
				if (Game::getOption(DRAW_DEBUG_OVERLAY))
				{
					if (currentEvent.key.code == sf::Keyboard::Key::Up)
					{
						Game::debugOverlay.consoleUP();
					}else if (currentEvent.key.code == sf::Keyboard::Key::Down)
					{
						Game::debugOverlay.consoleDOWN();
					}
				}
				handleKeyPress(currentEvent.key);
				break;
			}
			case(sf::Event::EventType::KeyReleased) :
			{
				handleKeyRelease(currentEvent.key);
				break;
			}
			case(sf::Event::EventType::MouseButtonPressed) :
			{
				handleMouseKeyPress(currentEvent.mouseButton);
				break;
			}
			case(sf::Event::EventType::MouseWheelMoved) :
			{
				handleMouseWheelSroll(currentEvent.mouseWheel);
				break;
			}
			case(sf::Event::EventType::Closed) :
			{
				Game::gameState = Game::GameState::Exiting;
				break;
			}
			case(sf::Event::EventType::Resized) :
			{
				Game::cameraManager.getCurrCamera()->setViewport(0, 0, currentEvent.size.width, currentEvent.size.height);
				CURR_CAM->setViewport(0, 0, currentEvent.size.width, currentEvent.size.height);
				Game::ScreenSizeX = currentEvent.size.width; Game::ScreenSizeY = currentEvent.size.height;
				break;
			}
			case(sf::Event::EventType::TextEntered):
			{
				if (Game::getOption(DRAW_DEBUG_OVERLAY))
				{
					if (currentEvent.text.unicode < 128)
					{
						Game::debugOverlay.consoleWrite(static_cast<char>(currentEvent.text.unicode));
					}
				}
			}
		}
	}
	//handleStateBased();
}

void EventManager::handleStateBased()
{	
	if (!Game::getOption(DRAW_DEBUG_OVERLAY))
	{
		currentCallbackSet->handleKeyState();
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			Game::setOption(DRAW_DEBUG_OVERLAY, !Game::getOption(DRAW_DEBUG_OVERLAY));
		}
	}
}

void EventManager::handleKeyPress(sf::Event::KeyEvent pKeyEvent)
{
	if (currentCallbackSet != nullptr)
	{
		currentCallbackSet->handleKeyPress(pKeyEvent);
	}

	if (!Game::getOption(DRAW_DEBUG_OVERLAY))
	{
		switch (pKeyEvent.code)
		{
		case(sf::Keyboard::Escape) :
		{
			if (Game::gameState == Game::GameState::Playing)
			{
				Game::gameState = Game::GameState::ShowingMenu;
			}
			else
			{
				Game::gameState = Game::GameState::Playing;
			}
			break;
		}
		
		case(sf::Keyboard::T) :
		{
			Game::setOption(DRAW_STRUCT_BOUNDS, !Game::getOption(DRAW_STRUCT_BOUNDS));
			break;
		}

		case(sf::Keyboard::Y) :
		{
			Game::setOption(DRAW_FPS, !Game::getOption(DRAW_FPS));
			break;
		}

		case(sf::Keyboard::U) :
		{
			Game::setOption(DRAW_INTERPOLATE, !Game::getOption(DRAW_INTERPOLATE));
			break;
		}

		case(sf::Keyboard::Tilde) :
		{
			Game::setOption(DRAW_DEBUG_OVERLAY, !Game::getOption(DRAW_DEBUG_OVERLAY));
			break;
		}

		case(sf::Keyboard::Q) :
		{
			Game::setOption(DRAW_QUADTREE_BOUNDS, !Game::getOption(DRAW_QUADTREE_BOUNDS));
			break;
		}

		case(sf::Keyboard::R) :
		{
			Game::setOption(DRAW_CONVEX_HULL, !Game::getOption(DRAW_CONVEX_HULL));
			break;
		}

		case(sf::Keyboard::O) :
		{
			Game::setOption(DRAW_RAY_INTERSECTS, !Game::getOption(DRAW_RAY_INTERSECTS));
			break;
		}
		
		case(sf::Keyboard::P) :
		{
			break;
		}

		case(sf::Keyboard::L) :
		{
			break;
		}

		case(sf::Keyboard::K) :
		{
			break;
		}

		case(sf::Keyboard::C) :
		{
			CURR_CAM->toggleConstrain();
			break;
		}
		}
	}
	else
	{
		switch (pKeyEvent.code)
		{
		case(sf::Keyboard::Tilde) :
		{
			Game::setOption(DRAW_DEBUG_OVERLAY, !Game::getOption(DRAW_DEBUG_OVERLAY));
			break;
		}
		}
	}
}

void EventManager::handleKeyRelease(sf::Event::KeyEvent pKeyEvent)
{
	if (currentCallbackSet != nullptr)
	{
		currentCallbackSet->handleKeyRelease(pKeyEvent);
	}

	if (!Game::getOption(DRAW_DEBUG_OVERLAY))
	{
		switch (pKeyEvent.code)
		{
		case(sf::Keyboard::Space) :
		{
			//Game::player->spaceReleasedAfterJump = true;
			break;
		}
		}
	}
}

void EventManager::handleMouseKeyPress(sf::Event::MouseButtonEvent pMouseEvent)
{
	currentCallbackSet->handleMousePress(pMouseEvent);
}

void EventManager::handleMouseKeyRelease(sf::Event::MouseButtonEvent pMouseEvent)
{

}

void EventManager::handleMouseWheelSroll(sf::Event::MouseWheelEvent pWheelEvent)
{
	if (currentEvent.mouseWheel.delta > 0)
	{
		vec2d size = CURR_CAM->getSize();
		//cameraManager.getCurrCamera()->setSize(size.x + 128, size.y + 72);
		CURR_CAM->zoom(1.1f);
	}
	else
	{
		vec2d size = CURR_CAM->getSize();
		//cameraManager.getCurrCamera()->setSize(size.x - 128, size.y - 72);
		CURR_CAM->zoom(0.9f);
	}
}
