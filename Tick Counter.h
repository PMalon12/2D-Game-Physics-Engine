#pragma once
#include "include.h"
#include "QPC.h"

class TickCounter
{
public:
	TickCounter();
	~TickCounter();

	void initialise();
	void drawFPS();

	void updateElapsedTime();
	void updateProgramTime();
	void updateTicksPerSecond();

	unsigned long long getElapsedTime();
	int getTicksPerSecond();
	const unsigned long long getProgramTime() { return programTime; }

	void setTextPos(vec2f pPos);

	unsigned long long elapsedTime;

private:

	static QPC qpc;

	std::vector<unsigned long long> hist;
	int hI;

	unsigned long long programTime;
	unsigned long long lastET;
	double FPS;
};