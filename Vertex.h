#pragma once

class Vertex
{
public:
	Vertex() : x(0.0f), y(0.0f), z(-1.0f)/*, r(1.0f), g(1.0f), b(1.0f), a(1.0f), s0(1.0f), t0(1.0f)*/ {}
	Vertex(vec2f pXY) : x(pXY.x), y(pXY.y), z(-1.0f)/*, r(1.0f), g(1.0f), b(1.0f), a(1.0f), s0(1.0f), t0(1.0f)*/ {}
	Vertex(float px, float py) : x(px), y(py) {}
	~Vertex() {}

	float x, y, z;
	//float r, g, b, a;
	float s0, t0;
};

class Vertex2D
{
public:
	Vertex2D() : x(0.f), y(0.f) {}
	Vertex2D(float px, float py) : x(px), y(py) {}
	Vertex2D(float px, float py, float ps, float pt) : x(px), y(py), s0(ps), t0(pt) {}
	Vertex2D(vec2f p) : x(p.x), y(p.y) {}
	Vertex2D(vec2f p, vec2f t) : x(p.x), y(p.y), s0(p.x), t0(p.y) {}
	~Vertex2D() {}

	union {
		struct { float x, y; };
		vec2f pos;
	};
	float s0, t0;
};