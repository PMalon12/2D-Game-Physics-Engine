#pragma once
#include <functional>

class Physical;

class ContactManifold;

typedef std::function<void(Physical*, Physical*, const ContactManifold&)> CollisionCallback;
typedef std::function<bool(Physical*, const Physical*, const ContactManifold&)> CollisionCriteria;

class CCBAC //Collision CallBacks And Criteria
{
public:
	CCBAC() {}
	~CCBAC() {}

	static void playerCollisionCallback(Physical*, Physical*, const ContactManifold&);
	static bool playerCollisionCriteria(Physical*, const Physical*, const ContactManifold&);
};