#pragma once
#include "Include.h"

class Game;

class CallbackCollection
{
public:
	CallbackCollection() {}
	~CallbackCollection() {}

	struct KeyEventWrapper
	{
		KeyEventWrapper(sf::Keyboard::Key pKey, bool pControl = false, bool pAlt = false, bool pShift = false, bool pSystem = false)
		{
			keyData.code = pKey; keyData.control = pControl; keyData.alt = pAlt; keyData.shift = pShift; keyData.system = pSystem;
		}

		KeyEventWrapper(sf::Event::KeyEvent ke) : keyData(ke) {}

		bool operator==(const KeyEventWrapper& rhs)
		{
			return (rhs.keyData.code == keyData.code) && (rhs.keyData.alt == keyData.alt) && (rhs.keyData.control == keyData.control) && (rhs.keyData.shift == keyData.shift) && (rhs.keyData.system == keyData.system);
		}

		sf::Event::KeyEvent keyData;
	};

	struct KeyCallback
	{
		KeyCallback(std::function<void(void*)> pFunc, KeyEventWrapper pKeyData, void* src, int pCBID) : callback(pFunc), keyData(pKeyData), source(src), callbackID(pCBID) {}
		KeyCallback(KeyEventWrapper pKeyData, void* src, int pCBID) : keyData(pKeyData), source(src), callbackID(pCBID) {}
		std::function<void(void*)> callback;
		KeyEventWrapper keyData;
		void* source;
		int callbackID;

		bool operator==(KeyCallback& rhs)
		{
			return (keyData == rhs.keyData && source == rhs.source && callbackID == rhs.callbackID);
		}
	};

	struct MouseEventWrapper
	{
		MouseEventWrapper(sf::Event::MouseButtonEvent pMouseData) : mouseData(pMouseData) {}
		MouseEventWrapper(sf::Mouse::Button pMouseButton) { mouseData.button = pMouseButton; }
		sf::Event::MouseButtonEvent mouseData;

		bool operator==(const MouseEventWrapper& rhs)
		{
			return mouseData.button == rhs.mouseData.button;
		}
	};

	struct MouseCallback
	{
		MouseCallback(std::function<void(void*)> pFunc, MouseEventWrapper pMouseData, void* src, int pCBID) : callback(pFunc), mouseData(pMouseData), source(src), callbackID(pCBID) {}
		MouseCallback(MouseEventWrapper pMouseData, void* src, int pCBID) : mouseData(pMouseData), source(src), callbackID(pCBID) {}
		std::function<void(void*)> callback;
		MouseEventWrapper mouseData;
		void* source;
		int callbackID;

		bool operator==(MouseCallback& rhs)
		{
			return (mouseData == rhs.mouseData && source == rhs.source && callbackID == rhs.callbackID);
		}
	};

	std::list<KeyCallback> keyPressCallbacks;
	std::list<KeyCallback> keyReleaseCallbacks;
	std::list<MouseCallback> mousePressCallbacks;
	std::list<MouseCallback> mouseReleaseCallbacks;
	std::list<KeyCallback> keyStateCallbacks;

	void clearCallbacks()
	{
		keyPressCallbacks.clear();
		keyReleaseCallbacks.clear();
		mousePressCallbacks.clear();
		mouseReleaseCallbacks.clear();
		keyStateCallbacks.clear();
	}

	void handleKeyState() 
	{
		for (auto itr = keyStateCallbacks.begin(); itr != keyStateCallbacks.end(); ++itr)
		{
			if (sf::Keyboard::isKeyPressed(itr->keyData.keyData.code))
			{
				if (itr->keyData.keyData.alt)
					if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)))
						continue;
				if (itr->keyData.keyData.control)
					if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)))
						continue;
				if(itr->keyData.keyData.shift)
					if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
						continue;
				if (itr->keyData.keyData.system)
					if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem) || sf::Keyboard::isKeyPressed(sf::Keyboard::RSystem)))
						continue;

				itr->callback(itr->source);
			}
		}
	}

	void addKeyStateCallback(std::function<void(void*)> pFunc, KeyEventWrapper pKeyData, void* source, int callbackID)
	{
		keyStateCallbacks.push_back(KeyCallback(pFunc, pKeyData, source, callbackID));
	}
	void removeKeyStateCallback(KeyEventWrapper pKeyData, void* source, int callbackID)
	{
		for (auto itr = keyStateCallbacks.begin(); itr != keyStateCallbacks.end();)
		{
			if (*itr == KeyCallback(pKeyData, source, callbackID))
			{
				itr = keyStateCallbacks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	void handleKeyPress(sf::Event::KeyEvent ke)
	{
		for (auto itr = keyPressCallbacks.begin(); itr != keyPressCallbacks.end(); ++itr)
		{
			if (itr->keyData == KeyEventWrapper(ke))
			{
				itr->callback(itr->source);
			}
		}
	}
	void handleKeyRelease(sf::Event::KeyEvent ke)
	{
		for (auto itr = keyReleaseCallbacks.begin(); itr != keyReleaseCallbacks.end(); ++itr)
		{
			if (itr->keyData == KeyEventWrapper(ke))
			{
				itr->callback(itr->source);
			}
		}
	}

	void addKeyPressCallback(std::function<void(void*)> pFunc, KeyEventWrapper pKeyData, void* source, int callbackID)
	{
		keyPressCallbacks.push_back(KeyCallback(pFunc, pKeyData, source, callbackID));
	}
	void removeKeyPressCallback(KeyEventWrapper pKeyData, void* source, int callbackID)
	{
		for (auto itr = keyPressCallbacks.begin(); itr != keyPressCallbacks.end();)
		{
			if (*itr == KeyCallback(pKeyData, source, callbackID))
			{
				itr = keyPressCallbacks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
	void addKeyReleaseCallback(std::function<void(void*)> pFunc, KeyEventWrapper pKeyData, void* source, int callbackID)
	{
		keyReleaseCallbacks.push_back(KeyCallback(pFunc, pKeyData, source, callbackID));
	}
	void removeKeyReleaseCallback(KeyEventWrapper pKeyData, void* source, int callbackID)
	{
		for (auto itr = keyReleaseCallbacks.begin(); itr != keyReleaseCallbacks.end();)
		{
			if (*itr == KeyCallback(pKeyData, source, callbackID))
			{
				itr = keyReleaseCallbacks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	void handleMousePress(sf::Event::MouseButtonEvent me)
	{
		for (auto itr = mousePressCallbacks.begin(); itr != mousePressCallbacks.end(); ++itr)
		{
			if (itr->mouseData == me)
			{
				itr->callback(itr->source);
			}
		}
	}
	void handleMouseRelease(sf::Event::MouseButtonEvent me)
	{
		for (auto itr = mouseReleaseCallbacks.begin(); itr != mouseReleaseCallbacks.end(); ++itr)
		{
			if (itr->mouseData == me)
			{
				itr->callback(itr->source);
			}
		}
	}

	void addMousePressCallback(std::function<void(void*)> pFunc, MouseEventWrapper pKeyData, void* source, int callbackID)
	{
		mousePressCallbacks.push_back(MouseCallback(pFunc, pKeyData, source, callbackID));
	}
	void removeMousePressCallback(MouseEventWrapper pKeyData, void* source, int callbackID)
	{
		for (auto itr = mousePressCallbacks.begin(); itr != mousePressCallbacks.end();)
		{
			if (*itr == MouseCallback(pKeyData, source, callbackID))
			{
				itr = mousePressCallbacks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
	void addMouseReleaseCallback(std::function<void(void*)> pFunc, MouseEventWrapper pKeyData, void* source, int callbackID)
	{
		mouseReleaseCallbacks.push_back(MouseCallback(pFunc, pKeyData, source, callbackID));
	}
	void removeMouseReleaseCallback(MouseEventWrapper pKeyData, void* source, int callbackID)
	{
		for (auto itr = mouseReleaseCallbacks.begin(); itr != mouseReleaseCallbacks.end();)
		{
			if (*itr == MouseCallback(pKeyData, source, callbackID))
			{
				itr = mouseReleaseCallbacks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
};

typedef CallbackCollection::KeyEventWrapper KeyEventWrap;
typedef CallbackCollection::MouseEventWrapper MouseEventWrap;

class EventManager
{
	friend class Game;
public:

	void handleEvents();
	void handleStateBased();
	void handleKeyPress(sf::Event::KeyEvent pKeyEvent);
	void handleKeyRelease(sf::Event::KeyEvent pKeyEvent);
	void handleMouseKeyPress(sf::Event::MouseButtonEvent pMouseEvent);
	void handleMouseKeyRelease(sf::Event::MouseButtonEvent pMouseEvent);
	void handleMouseWheelSroll(sf::Event::MouseWheelEvent pWheelEvent);

	void setCurrentCallbackSet(CallbackCollection* cb) { currentCallbackSet = cb; }

private:

	sf::Event currentEvent;

	CallbackCollection* currentCallbackSet;
};
