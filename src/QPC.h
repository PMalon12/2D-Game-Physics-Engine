#pragma once
#include <Windows.h>


class QPCWrapper
{
public:
	QPCWrapper() {}
	~QPCWrapper() {}

	void start()
	{
		QueryPerformanceCounter(&StartingTime);
		QueryPerformanceFrequency(&Frequency);
	}

	long long getElapsedTime()
	{
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

		ElapsedMicroseconds.QuadPart *= 1000000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

		return ElapsedMicroseconds.QuadPart;
	}

private:

	LARGE_INTEGER StartingTime;
	LARGE_INTEGER EndingTime;
	LARGE_INTEGER ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

};

#define QPC QPCWrapper
