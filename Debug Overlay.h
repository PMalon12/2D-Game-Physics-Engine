#pragma once
#include "Include.h"
#include "Text.h"
#include "Camera Manager.h"
#include "Console.h"

class DebugOverlay
{
	friend class Game;
public:
	DebugOverlay();
	~DebugOverlay() 
	{
		glDeleteBuffers(1, &boxVBO);
	}

	struct TimedElement
	{
		double elapsedTime;
		std::string label;
	};

	void draw();

	void consoleDOWN() // UP ARROW
	{
		if (inp.size() > 1)
		{
			if (cur == inp.begin())
			{
				cmd.back() = *cur;
			}
			else
			{
				cmd.back() = *(--cur);
			}
		}
	}

	void consoleUP() // DOWN ARROW
	{
		if (inp.size() > 1)
		{
			if (++cur == inp.end())
			{
				cmd.back() = *(--cur);
			}
			else
			{
				cmd.back() = *(cur);
			}
		}
	}

	void consoleWrite(char c)
	{
		switch (c)
		{
			case('\b') : // BACKSPACE
			{
				//if (cur->size() != 0)
				//{
				//	cur->erase(cur->end() - 1);
				//}
				
				if (cmd.back().size() > 2)
				{
					cmd.back().erase(cmd.back().end() - 1);
				}
				break;
			}
			case(13) : // ENTER
			{
				if (cmd.size() < 22)
				{
					inp.erase(inp.begin());
					inp.push_front(cmd.back());
					inp.push_front(std::string("> "));
					cmd.push_back(Console::interpretCommand(std::string(cmd.back().begin() + 2, cmd.back().end())));
					cmd.push_back(std::string("> "));
					cur = inp.begin();
				}
				else
				{
					inp.erase(inp.begin());
					inp.push_front(cmd.back());
					inp.push_front(std::string("> "));
					cmd.push_back(Console::interpretCommand(std::string(cmd.back().begin() + 2, cmd.back().end())));
					cmd.erase(cmd.begin());
					cmd.erase(cmd.begin());
					cmd.push_back(std::string("> "));
					cur = inp.begin();
				}

				std::cout << inp.front() << " " << cmd.front() << std::endl;

				//cur = cmd.begin();
				break;
			}
			case(96):
			case(126): // TILDEs -- DO NOTHING 
				break;
			case(127) : // DELETE
			{

				break;
			}
			default:
			{
				if (c > 31)
				{
					//*cur += c;
					cmd.back() += c;
				}
			}
		}


	}

private:

	std::list<std::string> inp;
	std::vector<std::string> cmd;
	std::list<std::string>::iterator cur;

	void init();

	Text* command;
	Text* fps;
	Text* fpsP;
	GLuint boxVBO;
	Camera* cam;

	Shader* shader;

};