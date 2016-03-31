#pragma once
#include "FreeType Manager.h"

FTManager::FTManager() {}

FTManager::~FTManager() { FT_Done_FreeType(fTLib); }

void FTManager::init()
{
	if (FT_Init_FreeType(&fTLib)) {
		fprintf(stderr, "Could not init freetype library\n");
	}
	std::cout << "FreeType manager initialised" << std::endl;
}