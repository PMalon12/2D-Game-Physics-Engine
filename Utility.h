#pragma once
#include <cstddef>
#include <algorithm>

inline float invsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);             // what the fuck? 
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration

	return y;
}

inline float clamp(float val, float low, float high)
{
	return std::max(low, std::min(val, high));
}

inline uint32_t setBit(uint32_t& num, uint8_t bit, uint8_t val)
{
	num ^= (-val ^ num) & (1 << bit);
	return num;
}

inline bool getBit(uint32_t num, uint8_t bit)
{
	return (num >> bit) & 1;
}

#define VERT_NULL 0
#define VERT_2D 1
#define VERT_2D_TEX 2
#define VERT_2D_TEX_COL 3
#define VERT_2D_COL 4

#define VERT_3D 5
#define VERT_3D_TEX 6
#define VERT_3D_TEX_COL 7
#define VERT_3D_COL 8


template<class T>
T* ptr(T& ref) { return &ref; }

template<class T>
T* ptr(T* ptr) { return ptr; }

//inline float lerp(float v0, float v1, float t);
//
//inline vec2f vlerp(vec2f v0, vec2f v1, float t);