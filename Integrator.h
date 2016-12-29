#pragma once
#include "Include.h"
#include "State.h"

class Integrator
{
public:
	Integrator() {}
	~Integrator() {}

	static void RK4(State &state, float t, float dt, std::function<vec2f(State&, float t)> force, std::function<float(State&, float t)> torque)
	{
		integrateRK4(state, t, dt, force, torque);
	}

	static void euler(State &state, float t, float dt, std::function<vec2f(State&, float t)>& force, std::function<float(State&, float t)>& torque)
	{
		state.position += state.velocity * dt;
		state.momentum += force(state, t) * dt;
		state.velocity = state.momentum * state.inverseMass;

		state.rotation += state.angularVelocity * dt;
		state.angularMomentum += torque(state, t) * dt;
		state.angularVelocity = state.angularMomentum * state.inverseInertiaMoment;
	}

	static Derivative evaluate(State &initial, float t, std::function<vec2f(State&, float)>& force)
	{
		Derivative output;
		State state = initial;
		output.dx = state.velocity;
		output.dp = force(state, t);
		return output;
	}

	static Derivative evaluate(State &initial, float t, float dt, Derivative &d, std::function<vec2f(State&, float)>& force)
	{
		State state;
		state.position = initial.position + d.dx*dt;
		state.velocity = initial.momentum * initial.inverseMass;
		state.mass = initial.mass;
		Derivative output;
		output.dx = state.velocity;
		output.dp = force(state, t + dt);
		return output;
	}

	static Derivative evaluateang(State &initial, float t, std::function<float(State&, float)>& torque)
	{
		Derivative output;
		State state = initial;
		output.dtheta = state.angularVelocity;
		output.dpang = torque(state, t);
		return output;
	}

	static Derivative evaluateang(State &initial, float t, float dt, Derivative &d, std::function<float(State&, float)>& torque)
	{
		State state;
		state.angularMomentum = initial.angularMomentum + d.dpang*dt;
		state.angularVelocity = initial.angularMomentum * initial.inverseInertiaMoment;
		state.inertiaMoment = initial.inertiaMoment;
		Derivative output;
		output.dtheta = state.angularVelocity;
		output.dpang = torque(state, t + dt);
		return output;
	}

	static void integrateRK4(State &state, float t, float dt, std::function<vec2f(State&, float)>& force, std::function<float(State&, float)>& torque)
	{
		Derivative a = evaluate(state, t, force);
		Derivative b = evaluate(state, t, dt * 0.5f, a, force);
		Derivative c = evaluate(state, t, dt * 0.5f, b, force);
		Derivative d = evaluate(state, t, dt, c, force);

		vec2f dxdt;
		dxdt.x = 1.0f / 6.0f * (a.dx.x + 2.0f*(b.dx.x + c.dx.x) + d.dx.x);
		dxdt.y = 1.0f / 6.0f * (a.dx.y + 2.0f*(b.dx.y + c.dx.y) + d.dx.y);
		vec2f dpdt;
		dpdt.x = 1.0f / 6.0f * (a.dp.x + 2.0f*(b.dp.x + c.dp.x) + d.dp.x);
		dpdt.y = 1.0f / 6.0f * (a.dp.y + 2.0f*(b.dp.y + c.dp.y) + d.dp.y);

		//vec2f dxdt = state.velocity;
		//vec2f dpdt = force(state, t);

		state.position = state.position + dxdt*dt;
		state.momentum = state.momentum + dpdt*dt;
		state.velocity = state.momentum * state.inverseMass;

		a = evaluateang(state, t, torque);
		b = evaluateang(state, t, dt*0.5f, a, torque);
		c = evaluateang(state, t, dt*0.5f, b, torque);
		d = evaluateang(state, t, dt, c, torque);

		float dpangdt;
		dpangdt = 1.0f / 6.0f * (a.dpang + 2.0f*(b.dpang + c.dpang) + d.dpang);
		//dpangdt = state.angularVelocity;
		float dthetadt;
		//dthetadt = torque(state, t);
		dthetadt = 1.0f / 6.0f * (a.dtheta + 2.0f*(b.dtheta + c.dtheta) + d.dtheta);

		state.rotation = state.rotation + dthetadt*dt;
		state.angularMomentum = state.angularMomentum + dpangdt*dt;
		state.angularVelocity = state.angularMomentum * state.inverseInertiaMoment;
	}

};