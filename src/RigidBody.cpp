#pragma once
#include "RigidBody.h"
#include "Game.h"
#include "Integrator.h"
#include "State.h"

void RigidBody::updateRigidBody(float dt, Transformable& transform, std::function<vec2f(State&, float)> force, std::function<float(State&, float)> torque)
{
	//Apply over time forces
	for (auto itr = tForces.begin(); itr != tForces.end();)
	{
		applyForce(itr->force * std::min((dt / itr->totalTime),1.f), vec2f(0, 0));
		itr->timeLeft -= dt;
		if (itr->timeLeft < 0.f)
			itr = tForces.erase(itr);
		else
			++itr;
	}

	//Integrate velocity and position
	Integrator::euler(state, Game::tickCounter.getProgramTime() * TOSECONDS, dt, force, torque);
	//Integrator::RK4(state, Game::tickCounter.getProgramTime() * TOSECONDS, dt, force, torque);

	//Set new position and rotation
	transform.getTransform().setTranslation(state.position);
	transform.getTransform().setRotation(state.rotation);
}