#include "Particle.h"
#include "Game.h"

Particle::Particle(Texture * a, float rad)
{
	dead = false;

	int num = 32;
	Triangle* trisss = new Triangle[num];

	for (int i = 0; i < num; ++i)
	{
		vec2f nextA(rad*std::sin((-i*PI * 2) / num), rad*std::cos((-i*PI * 2) / num));
		vec2f nextB(rad*std::sin(((-i + 1)*PI * 2) / num), rad*std::cos(((-i + 1)*PI * 2) / num));
		vec2f nAN(nextA + rad); nAN = nAN / (rad * 2);
		vec2f nBN(nextB + rad); nBN = nBN / (rad * 2);
		trisss[i].setVertices(Vertex2D(0, 0, 0.5, 0.5),
			Vertex2D(nextA.x, nextA.y, nAN.x, nAN.y),
			Vertex2D(nextB.x, nextB.y, nBN.x, nBN.y));
	}
	mesh = new TriangleMesh(num, trisss);

	updateBounds();
	affectedByGravity = false;

	static CollisionCriteria cr = [](Physical* __this, const Physical* collider, const ContactManifold& manifold) -> bool
	{
		auto _this = reinterpret_cast<Particle*>(__this);
		return (getBit(collider->getID(), PHYSICAL_BIT) && collider->getID() != _this->getID());
	};

	registerCollisionCriteria(cr);

	lifeLeft.setSeconds(0.7);

	pushBackStrength = 0.2;
	restitution = 0.3f;
	mesh->texture = a;
	setID(O_PROJECTILE);
}

void Particle::update(float dt)
{
	std::function<vec2f(State&, float)> force = [=](State& state, float t) -> vec2f { return vec2f(state.velocity.x * (-1), state.velocity.y * (-1)); };
	std::function<float(State&, float)> torque = [](State& pstate, float t) -> float { return -(50 * pstate.angularVelocity); };

	if (dead)
	{
		Game::map->removeEntity(this);
		return;
	}

	prevPos = getBodyPosition();
	prevRot = getBodyRotation();

	if (lifeLeft.removeMicroSeconds(Game::getDT().getMicroSeconds()) < 0)
	{
		dead = true;
	}

	updateBounds();

	updateRigidBody(dt * TOSECONDS, *this, force, torque);
}
