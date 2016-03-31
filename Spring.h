//#pragma once
//#include "Include.h"
//#include "Game.h"
//
//class Spring
//{
//public:
//	Spring() {}
//	~Spring() {}
//
//	void update(float dt)
//	{
//		//integrate(state, Game::tickCounter.getProgramTime() * TOSECONDS, dt * TOSECONDS);
//	}
//
//	void setAnchor(vec2f pPos) { anchor = pPos; }
//	vec2f getAnchor() { return anchor; }
//
//	void setPos(vec2f pPos) { state.x = pPos; }
//	vec2f getPos() { return state.x; }
//
//	void setRestLength(float pR) { restLength = pR; }
//	float getRestLength() { return restLength; }
//
//	void setMass(float pM) { mass = pM; }
//	float getMass() { return mass; }
//
//	void setStrength(float pK) { strength = pK; }
//	float getStrength() { return strength; }
//
//	void setDamping(float pB) { damping = pB; }
//	float getDamping() { return damping; }
//
//	void setGravity(float pG) { gravity = pG; }
//	float getGravity() { return gravity; }
//
//private:
//
//	struct State
//	{
//		vec2f x;
//		vec2f v;
//		float mass;
//		float inverseMass;
//	};
//
//	vec2f anchor;
//	float restLength;
//	float mass;
//	float strength;
//	float damping;
//	float gravity;
//
//	State state;
//
//	struct Derivative
//	{
//		vec2f dx;
//		vec2f dv;
//	};
//
//	vec2f acceleration(State &state, float t)
//	{
//		//return vec2f(((-(Game::k/Game::m)*(state.x.x) - (Game::b/Game::m)*(state.v.x)) - Game::g), (-(Game::k / Game::m)*(state.x.y) - (Game::b / Game::m)*(state.v.y)) - Game::g);
//
//		vec2f dir = (state.x - anchor);
//		if (dir.magSquared() > 0.1)
//		{
//			dir.normalise();
//		}
//		else
//		{
//			dir = vec2f(0.001, 0.001);
//		}
//
//		vec2f R;
//		R = vec2f(anchor.x + (dir.x * restLength), anchor.y + (dir.y * restLength));
//
//		float S = (state.x - (R)).mag();
//
//		float ctheta = (state.x.y - R.y) / S;
//		float stheta = (state.x.x - R.x) / S;
//
//		vec2f ret;
//		ret.x = ((-strength / mass) * stheta * S) - ((damping / mass) * state.v.x);
//		ret.y = -((strength / mass) * ctheta * S) - ((damping / mass) * state.v.y);
//		ret.y -= gravity;
//
//
//		return ret;
//	}
//};