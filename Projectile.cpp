#include "Projectile.h"
#include "Game.h"

Projectile::Projectile(Texture * a, float rad)
{
	dead = false;

	int num = 32;
	Triangle* trisss = new Triangle[num];

	for (int i = 0; i < num; ++i)
	{
		vec2f nextA(rad*std::sin((-i*PI * 2) / num), rad*std::cos((-i*PI * 2) / num));
		vec2f nextB(rad*std::sin(((-i + 1)*PI * 2) / num), rad*std::cos(((-i + 1)*PI * 2) / num));
		vec2f nAN(nextA + rad); nAN = nAN / (rad*2); //nAN = nAN / vec2f(a->getWidth(), a->getHeight());
		vec2f nBN(nextB + rad); nBN = nBN / (rad*2); //nBN = nBN / vec2f(a->getWidth(), a->getHeight());
		trisss[i].setVertices(Vertex2D(0, 0, 0.5, 0.5),
			Vertex2D(nextA.x, nextA.y, nAN.x, nAN.y),
			Vertex2D(nextB.x, nextB.y, nBN.x, nBN.y));
	}
	mesh = new TriangleMesh(num, trisss,Transform().rotate(PI/4));

	updateBounds();
	affectedByGravity = false;

	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		auto _this = reinterpret_cast<Projectile*>(__this);
		if (collider->getID() != O_PLAYER && collider->getID() != O_PROJECTILE)
			_this->lifeLeft.setSeconds(_this->lifeLeft.getSeconds()*0.5);
	};

	registerCollisionCallback(cb);

	static CollisionCriteria cr = [](Physical* __this, const Physical* collider, const ContactManifold& manifold) -> bool
	{
		auto _this = reinterpret_cast<Projectile*>(__this);
		return (collider != _this->parent && collider->getID() != _this->getID()); 
	};

	registerCollisionCriteria(cr);

	lifeLeft.setSeconds(0.7);

	staticFriction = 0.f;
	dynamicFriction = 0.f;
	pushBackStrength = 0.2;
	restitution = 0.5f;
	mesh->texture = a;
	setID(O_PROJECTILE);
}

void Projectile::update(float dt)
{
	std::function<vec2f(State&, float)> force = [=](State& state, float t) -> vec2f { return vec2f(state.velocity.x * (-1), state.velocity.y * (-1)); };
	std::function<float(State&, float)> torque = [](State& pstate, float t) -> float { return -(50 * pstate.angularVelocity); };

	if (dead)
	{
		Game::map->removeEntity(this);
		return;	
	}

	if (lifeLeft.removeMicroSeconds(Game::getDT().getMicroSeconds()) < 0)
	{
		dead = true;
	}

	updateBounds();

	updateRigidBody(dt * TOSECONDS, *this, force, torque);

	prevPos = getBodyPosition();
	prevRot = getBodyRotation();
}

void Projectile::init()
{
	parent = Game::map->getEntity(parUID);
}
