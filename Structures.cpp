#pragma once
#include "Structures.h"
#include "Game.h"
#include "Utility.cpp"

void Structure::draw(float alpha)
{
	Game::obDrawn += 1;
	Game::polyDrawn += mesh->totTris;

	mesh->draw(getTransform(), vec2f(0,0), GL_REPEAT, GL_REPEAT);
	mesh->drawConvexHull(getTransform());
	drawBounds(getTransform());

	if (Game::getOption(DRAW_CONVEX_HULL))
	{
		updateBounds(); //TODO: REMOVE THIS
		//uid.draw();
	}

	auto shader = Game::shaderManager.getShader("minTex");
	auto texoffs = glGetUniformLocation(shader->getID(), "texCoordOffset");
	glUniform2f(texoffs, 0, 0);
}

void DynamicStructure::draw(float a)
{
	Game::obDrawn += 1;
	Game::polyDrawn += mesh->totTris;

	vec2f corPos = vlerp(prevPos, getBodyPosition(), a);
	float corRot = lerp(prevRot, getBodyRotation(), a);

	auto x = getTransform();
	x.setTranslation(corPos);
	x.setRotation(0);

	drawBounds(x);

	x.setRotation(corRot);

	mesh->draw(x, vec2f(0, 0), GL_REPEAT, GL_REPEAT);
	mesh->drawConvexHull(x);

	if (Game::getOption(DRAW_CONVEX_HULL))
	{
		updateBounds(); //TODO: REMOVE THIS
		//uid.draw();
	}
}

void DynamicStructure::update(float dt)
{
	prevPos = getPosition();
	prevRot = getRotation();

	std::function<vec2f(State&, float)> force = [=](State& state, float t) -> vec2f { return state.velocity * (-30); };
	std::function<float(State&, float)> torque = [](State& pstate, float t) -> float { return -(5000 * pstate.angularVelocity); };

	updateBounds();
	
	updateRigidBody(dt*TOSECONDS, *this, force, torque);
}

LavaStructure::LavaStructure(std::ifstream & ifs) : StaticStructure(ifs) 
{
	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		collider->dampen(10000);
	};
	registerCollisionCallback(cb);
}

LavaStructure::LavaStructure(TriangleMesh * pTMesh) : StaticStructure(pTMesh)
{
	setID(O_SS_LAVA);
	staticFriction = 1.0f;
	dynamicFriction = 1.0f;
	pushBackStrength = -0.3f;
	
	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		collider->dampen(10000);
	};
	registerCollisionCallback(cb);
}

void LavaStructure::draw(float a)
{
	vec2f texoffs(std::sin(Game::tickCounter.getProgramTime() * TOSECONDS) * 0.6f, std::cos(Game::tickCounter.getProgramTime() * TOSECONDS) * 0.3f);
	mesh->draw(getTransform(), texoffs, GL_REPEAT, GL_REPEAT);
	if (Game::getOption(DRAW_CONVEX_HULL))
	{
		uid.setColour(Colour(0.0, 1.0, 0.0, 1.0));
		updateBounds(); //TODO: REMOVE THIS
		uid.draw();
	}
}

void Checkpoint::setIndex(int i)
{
	deregisterCollisionCallbacks();
	checkpointIndex = i;

	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		if (collider->getID() == O_PLAYER)
		{
			auto _this = static_cast<Checkpoint*>(__this);
			if (Game::map->checkpointTrigger(_this->checkpointIndex, _this->getGlobalBounds().mid(), _this))
			{
				_this->mesh->r = 0.0;
				_this->mesh->g = 1.0;
			}
		}
	};

	registerCollisionCallback(cb);
}

void Checkpoint::draw(float a)
{
	mesh->draw(getTransform(), vec2f(0, 0), GL_REPEAT, GL_REPEAT);
	mesh->drawConvexHull(getTransform());
	drawBounds(getTransform());

	if (Game::getOption(DRAW_CONVEX_HULL))
	{
		updateBounds(); //: REMOVE THIS
		uid.draw();
	}
}

EndLevel::EndLevel(std::ifstream & ifs) : StaticStructure(ifs)
{
	loadFromStream(ifs);
	setID(O_END_LEVEL);
	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			auto _this = reinterpret_cast<EndLevel*>(__this);
			Game::changeMap(std::string(_this->nextLevel, 32));
		}
	};
	registerCollisionCallback(cb);
}

EndLevel::EndLevel(TriangleMesh * pTMesh) : StaticStructure(pTMesh)
{
	setID(O_END_LEVEL);
	nextLevel[0] = '\0';
	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			auto _this = reinterpret_cast<EndLevel*>(__this);
			Game::changeMap(std::string(_this->nextLevel, 32));
		}
	};
	registerCollisionCallback(cb);
}

Powerup::Powerup()
{
	auto tri = new Triangle[2];
	tri[0].setVertices(Vertex2D(-32, -32, 0, 1), Vertex2D(32, 32, 1, 0), Vertex2D(-32, 32, 0, 0));
	tri[1].setVertices(Vertex2D(-32, -32, 0, 1), Vertex2D(32, -32, 1, 1), Vertex2D(32, 32, 1, 0));
	auto tm = new TriangleMesh(2, tri);
	mesh = tm;
	noMesh = 1;
	updateBounds();
	setID(O_POWERUP);

	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		if (collider->getID() == O_PLAYER)
		{
			auto _this = reinterpret_cast<Powerup*>(__this);
			auto player = reinterpret_cast<Player*>(collider);
			player->givePower(_this->powerCode);
			Game::map->removeStructure(_this);
		}
	};
	registerCollisionCallback(cb);
}

Powerup::Powerup(std::ifstream & ifs) : StaticStructure(ifs) {
	loadFromStream(ifs);
	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		if (collider->getID() == O_PLAYER)
		{
			auto _this = reinterpret_cast<Powerup*>(__this);
			auto player = reinterpret_cast<Player*>(collider);
			player->givePower(_this->powerCode);
			Game::map->removeStructure(_this);
		}
	};
	registerCollisionCallback(cb);
}

void Powerup::draw(float alpha)
{
	auto t = getTransform();
	t.translate(vec2f(0,std::sin(Game::tickCounter.getProgramTime() * TOSECONDS * 2))*10);
	mesh->draw(t);
	drawBounds(getTransform());
	mesh->drawConvexHull(getTransform());
}
