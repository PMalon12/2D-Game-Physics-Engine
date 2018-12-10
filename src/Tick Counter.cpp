#pragma once
#include "tick counter.h"
#include "game.h"

TickCounter::TickCounter() : FPS(0), hI(0) { hist.resize(5); hist.reserve(5); }

TickCounter::~TickCounter() {}

void TickCounter::initialise()
{
	programTime = 0.0;
	qpc.start();
}

unsigned long long TickCounter::getElapsedTime()
{
	return elapsedTime;
}

void TickCounter::updateProgramTime()
{
	programTime += qpc.getElapsedTime();
	qpc.start(); //RE-start
}

void TickCounter::updateElapsedTime()
{
	elapsedTime = programTime - lastET;
	hist[hI] = elapsedTime;
	hI = hI + 1 > 4 ? 0 : hI + 1;
	lastET = programTime;
}

void TickCounter::updateTicksPerSecond()
{
	FPS = (1.0 / elapsedTime) * 1000000.0;
}

void TickCounter::drawFPS()
{
}

int TickCounter::getTicksPerSecond()
{
	return (1.f / ((hist[0] + hist[1] + hist[2] + hist[3] + hist[4]) / 5)) * 1000000.0;
}

void TickCounter::setTextPos(vec2f pPos)
{
	//tpsText.setPosition(pPos);
}

QPC TickCounter::qpc;
