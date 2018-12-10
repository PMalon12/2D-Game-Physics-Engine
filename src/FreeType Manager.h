#pragma once
#include "Include.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class FTManager
{
	friend class Game;
public:
	FTManager();
	~FTManager();

	FT_Library fTLib;

private:
	void init();
};