#pragma once
#include "Include.h"
#include "Entity.h"

class NullEntity : public Entity
{
public:
	NullEntity() {}
	~NullEntity() {}

	void update(float dt) {}
	void draw(float alpha) {}

private:

};