#pragma once
#include "Include.h"
#include "Player.h"
#include "Game.h"
#include "Polytope.h"
#include "EPA.h"
#include "Utility.cpp"
#include "Projectile.h"
#include "Event Manager.h"

Player::Player()
{
	setPosition(vec2f(200, 200));
	auto size = vec2f(74, 256);
	auto origin = vec2f(37, 128);
	health = 100;

	for (int n : {0, 1, 2, 3, 4})
	{
		hasPower[n] = false;
	}

	Font* debugFont = Game::fontManager.getFont("consola");
	debug.setFont(debugFont);
	debug.setCharSize(18);
	debug.setColour(Colour(0, 0, 0));
	debug.setPosition(getBodyPosition());
	debug.setScale(vec2f(1, 1));

	Triangle* tris = new Triangle[2];
	tris[0].setVertices(Vertex2D(-origin.x, -origin.y, 1, 1), Vertex2D(-origin.x + size.x, -origin.y + size.y, 0, 0), Vertex2D(-origin.x, -origin.y + size.y, 1, 0));
	tris[1].setVertices(Vertex2D(-origin.x, -origin.y, 1, 1), Vertex2D(-origin.x + size.x, -origin.y, 0, 1), Vertex2D(-origin.x + size.x, -origin.y + size.y, 0, 0));

	float newEdgeVerts[] = { -origin.x, origin.y - 10,
							-origin.x, -origin.y + 2,
							-origin.x + 15, -origin.y,
							origin.x - 15, -origin.y,
							origin.x, -origin.y + 2,
							origin.x, origin.y - 10,
							origin.x - 10, origin.y,
							-origin.x + 10, origin.y};

	collisionVertsSize = 16;
	collisionVerts = new float[collisionVertsSize];
	std::memcpy(collisionVerts, newEdgeVerts, sizeof(newEdgeVerts));

	mesh = new TriangleMesh(2, tris);
	glow = new TriangleMesh(2, tris);
	glow->texture = Game::textureManager.getTexture("glow");
	
	//auto collisionOval = createRegularPolygon(36, 37, vec2f(1, 1), Transform().setScale(vec2f(1.f, 256.f / 74.f)));
	//col = collisionOval;
	//float* newEdgeVerts = new float[collisionOval->edgeVertDataSize];
	//std::memcpy(newEdgeVerts, collisionOval->edgeVertData, collisionOval->edgeVertDataSize * sizeof(float));
	//mesh->edgeVertData = newEdgeVerts;
	//mesh->edgeVertDataSize = collisionOval->edgeVertDataSize;

	mesh->buildConvexHull();
	setTexture(Game::textureManager.getTexture("char"));

	setBodyInertiaMoment(FLT_MAX);
	setBodyMass(40);
	restitution = 0.f;
	staticFriction = 0.f;
	dynamicFriction = 0.f;
	pushBackStrength = 0.2f;
	setID(O_PLAYER);
	setUID(0xfdcfdcfd);

	jumpCD = 0.00f;
	doubleJumpCD = 0.2f;
	glow->r = 0.f; glow->g = 0.f; glow->b = 0.f;
	mesh->r = 1.f; mesh->g = 1.f; mesh->b = 1.f;

	initt();
}

void Player::initt()
{
	registerCollisionCallback(CollisionCallback(CCBAC::playerCollisionCallback));
	registerCollisionCriteria(CollisionCriteria(CCBAC::playerCollisionCriteria));

	static CollisionCallback cb = [](Physical* __this, Physical* collider, const ContactManifold& manifold) -> void
	{
		if (!getBit(collider->getID(),PHYSICAL_BIT))
		{
			return;
		}

		auto _this = static_cast<Player*>(__this);

		vec2f steepest(_this->steepest);
		float steepestDot = 0;
		
		if (__this == manifold.A)
		{
			for (int i = 0; i < manifold.numContacts; ++i)
			{
				if (manifold.contacts[i].normal.dot(vec2f(0, -1)) < 0) continue;
				float dot = std::abs(manifold.contacts[i].normal.dot(vec2f(1, 0)));
				if (dot > steepestDot)
				{
					steepestDot = dot;
					steepest = manifold.contacts[i].normal;
				}
			}
		}
		else
		{
			for (int i = 0; i < manifold.numContacts; ++i)
			{
				if (manifold.contacts[i].normal.negative().dot(vec2f(0, -1)) < 0) continue;
				float dot = std::abs(manifold.contacts[i].normal.negative().dot(vec2f(1, 0)));
				if (dot > steepestDot)
				{
					steepestDot = dot;
					steepest = manifold.contacts[i].normal.negative();
				}
			}
		}

		if (steepest.inBetween(MAXWALKSLOPENEG, MAXWALKSLOPEPOS))
		{
			if (std::abs(steepest.dot(vec2f(1, 0))) > std::abs(_this->steepest.dot(vec2f(1, 0))))
			{
				_this->steepest = steepest;
			}
		}
	};

	registerCollisionCallback(cb);

	movForce = 200000;
	jumpStr = 120000;
	setID(O_PLAYER);
	setUID(0xfdcfdcfd);
	jumpCD = 0.00f;
	doubleJumpCD = 0.2f;
	curItem = 1;

	registerInputCallbacks();

	CURR_CAM->setFollowTarget(this);
}

void Player::registerInputCallbacks()
{
	static auto w = [](void* _this) -> void { static_cast<Player*>(_this)->move(vec2f(0, 1)); };
	static auto a = [](void* _this) -> void { static_cast<Player*>(_this)->move(vec2f(-1, 0)); };
	static auto s = [](void* _this) -> void { static_cast<Player*>(_this)->move(vec2f(0, -1)); };
	static auto d = [](void* _this) -> void { static_cast<Player*>(_this)->move(vec2f(1, 0)); };
	static auto j = [](void* _this) -> void { static_cast<Player*>(_this)->jump(); };
	static auto dj = [](void* _this) -> void { static_cast<Player*>(_this)->doubleJump(); };
	static auto jr = [](void* _this) -> void { static_cast<Player*>(_this)->spaceReleasedAfterJump = true; };

	static auto itm1 = [](void* _this) -> void { static_cast<Player*>(_this)->curItem = 1; };
	static auto itm2 = [](void* _this) -> void { static_cast<Player*>(_this)->curItem = 2; };
	static auto itm3 = [](void* _this) -> void { static_cast<Player*>(_this)->curItem = 3; };
	static auto itm4 = [](void* _this) -> void { static_cast<Player*>(_this)->curItem = 4; };
	static auto itm5 = [](void* _this) -> void { static_cast<Player*>(_this)->curItem = 5; };

	Game::cb.addKeyPressCallback(itm1, KeyEventWrap(sf::Keyboard::Num1), this, 0);
	Game::cb.addKeyPressCallback(itm2, KeyEventWrap(sf::Keyboard::Num2), this, 0);
	Game::cb.addKeyPressCallback(itm3, KeyEventWrap(sf::Keyboard::Num3), this, 0);
	Game::cb.addKeyPressCallback(itm4, KeyEventWrap(sf::Keyboard::Num4), this, 0);
	Game::cb.addKeyPressCallback(itm5, KeyEventWrap(sf::Keyboard::Num5), this, 0);

	//Game::cb.addKeyStateCallback(w, KeyEventWrap(sf::Keyboard::W), this, 0);
	Game::cb.addKeyStateCallback(a, KeyEventWrap(sf::Keyboard::A), this, 0);
	//Game::cb.addKeyStateCallback(s, KeyEventWrap(sf::Keyboard::S), this, 0);
	Game::cb.addKeyStateCallback(d, KeyEventWrap(sf::Keyboard::D), this, 0);
	Game::cb.addKeyStateCallback(j, KeyEventWrap(sf::Keyboard::Space), this, 0);
	Game::cb.addKeyPressCallback(dj, KeyEventWrap(sf::Keyboard::Space), this, 0);
	Game::cb.addKeyReleaseCallback(jr, KeyEventWrap(sf::Keyboard::Space), this, 0);

	Game::cb.addMousePressCallback(std::bind(&Player::fire, this), MouseEventWrap(sf::Mouse::Left), this, 0);
}

void Player::deregisterInputCallbacks()
{
	Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Num1), this, 0);
	Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Num2), this, 0);
	Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Num3), this, 0);
	Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Num4), this, 0);
	Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Num5), this, 0);

	//Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::W), this, 0);
	Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::A), this, 0);
	//Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::S), this, 0);
	Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::D), this, 0);
	Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::Space), this, 0);
	Game::cb.removeMousePressCallback(MouseEventWrap(sf::Mouse::Left), this, 0);

	Game::cb.removeKeyStateCallback(KeyEventWrap(sf::Keyboard::Space), this, 0);
	Game::cb.removeKeyPressCallback(KeyEventWrap(sf::Keyboard::Space), this, 0);
	Game::cb.removeKeyReleaseCallback(KeyEventWrap(sf::Keyboard::Space), this, 0);
}

Player::Player(std::ifstream& ifs) : Character(ifs)
{
	loadFromStream(ifs);
	initt();
}

Player::~Player()
{
	deregisterInputCallbacks();
}

void Player::update(float dt)
{
	std::function<vec2f(State&, float)> force = [=](State& state, float t) -> vec2f { return vec2f(state.velocity.x * (-100), state.velocity.y * (-10)); };
	std::function<float(State&, float)> torque = [](State& pstate, float t) -> float { return -(50 * pstate.angularVelocity); };

	const float stopSpeed = 800;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		applyLinearForce(dt * TOSECONDS * vec2f(getBodyVelocity().x * -stopSpeed, 0));

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && getBodyVelocity().x > 0)
		applyLinearForce(dt * TOSECONDS * vec2f(getBodyVelocity().x * -stopSpeed, 0));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && getBodyVelocity().x < 0)
		applyLinearForce(dt * TOSECONDS * vec2f(getBodyVelocity().x * -stopSpeed, 0));

	fm = std::sinf(Game::tickCounter.getProgramTime()*TOSECONDS * 2) * 20.f;
	gm = vec2f(0, fm);

	if (getBodyVelocity().x < -5)
	{
		if (texFace != Left)
		{
			mesh->setTextureCoord(0, vec2f(1, 1));
			mesh->setTextureCoord(1, vec2f(0, 0));
			mesh->setTextureCoord(2, vec2f(1, 0));
			mesh->setTextureCoord(3, vec2f(1, 1));
			mesh->setTextureCoord(4, vec2f(0, 1));
			mesh->setTextureCoord(5, vec2f(0, 0));
			mesh->bufferData();
		}

		texFace = Left;
	}
	else if (getBodyVelocity().x > 5)
	{
		if (texFace != Right)
		{
			mesh->setTextureCoord(0, vec2f(0, 1));
			mesh->setTextureCoord(1, vec2f(1, 0));
			mesh->setTextureCoord(2, vec2f(0, 0));
			mesh->setTextureCoord(3, vec2f(0, 1));
			mesh->setTextureCoord(4, vec2f(1, 1));
			mesh->setTextureCoord(5, vec2f(1, 0));
			mesh->bufferData();
		}

		texFace = Right;
	}

	if (texFace == Left)
	{
		gm += vec2f(17, 15);
	}
	else
	{
		gm += vec2f(-17, 15);
	}

	if (onGround)
	{
		mesh->r = 1.f; mesh->g = 1.f; mesh->b = 1.f;
		
		jumpTimer -= dt * TOSECONDS;
		jumpResetDelayTimer = jumpResetDelay;
		doubleJumpTimer = doubleJumpCD;
	}
	else
	{
		mesh->r = 0.9f; mesh->g = 0.9f; mesh->b = 0.9f;
		if (jumpResetDelayTimer < 0.f)
		{
			jumpTimer = jumpCD;
		}
		jumpResetDelayTimer -= dt * TOSECONDS;

		doubleJumpTimer -= dt * TOSECONDS;
	}

	if (primeDoubleJump && doubleJumpTimer < 0.f)
	{
		if (getBodyVelocity().y < 0)
			setBodyVelocity(vec2f(getBodyVelocity().x, 0));

		doubleJumpTimer = FLT_MAX;
		applyLinearImpulse(vec2f(0, 100000), 0.05);
		primeDoubleJump = false;
	}
	
	if (heldObject != nullptr)
	{
		auto mouse = Game::getWorldMousePos();
		heldObject->applyLinearForce(Game::map->getProperties().gravity * -0.5f);
		heldObject->applyLinearForce((mouse - heldObject->getBodyPosition())*10);
		//std::cout << (mouse - heldObject->getBodyPosition()).x << " : " << (mouse - heldObject->getBodyPosition()).y << std::endl;
	}

	if (steepest.inBetween(MAXWALKSLOPENEG, MAXWALKSLOPEPOS))
	{
		applyLinearForce(vec2f(0, std::abs(steepest.x) * 10000) * Game::map->getProperties().gravity.getNormalised().negate());
	}

	updateBounds();

	updateRigidBody(dt * TOSECONDS, *this, force, torque);

	//health -= dt * TOSECONDS * 1;
	debug.setString(std::to_string(health));
	debug.setPosition(getBodyPosition() + vec2f(50,0));
	onGround = false;
	jumpLock = false;
	steepest.x = 0; steepest.y = 0;
}

void Player::jump()
{
	spaceReleasedAfterJump = false;
	if (!jumpLock && jumpTimer < 0 && getBodyVelocity().y < 100)
	{
		applyLinearImpulse(vec2f(0, jumpStr), 0.05);
		jumpTimer = FLT_MAX;
	}
}

void Player::doubleJump()
{
	if (!spaceReleasedAfterJump || jumpLock || !hasPower[0]) return;

	if (jumpTimer > 0 && doubleJumpTimer < 0)
	{
		if (getBodyVelocity().y < 0)
			setBodyVelocity(vec2f(getBodyVelocity().x, 0));

		doubleJumpTimer = FLT_MAX;
		applyLinearImpulse(vec2f(0, jumpStr), 0.05);
		primeDoubleJump = false;
	}
	else if (doubleJumpTimer < doubleJumpCD / 2.f)
	{
		primeDoubleJump = true;
	}
}

void Player::move(vec2f pMove)
{
	applyLinearForce(pMove * movForce * Game::getDT().getSeconds());
}

void Player::fire()
{
	if (!hasPower[curItem]) { return; }
	switch (curItem)
	{
	case(1) :
	{
		vec2f dir;

		vec2f mouse = Game::getMousePos();
		glm::fvec3 unp = glm::unProject(glm::fvec3(mouse.x, mouse.y, 0), Game::cameraManager.getCurrCamera()->getViewMat(), Game::cameraManager.getCurrCamera()->getProjMat(), Game::cameraManager.getCurrCamera()->getViewport());
		dir = vec2f(unp.x - getBodyPosition().x, unp.y - getBodyPosition().y).normalise();

		dir.rotate(-PI / 8);
		dir.rotate(-PI / 16);
		for (int i = 0; i < 16; ++i)
		{
			auto np = new Projectile(Game::textureManager.getTexture("pmag"), 16);
			np->setPosition(getBodyPosition());
			np->applyLinearForce(dir.rotate(PI / 54) * 40000);
			np->setBodyMass(5);
			np->setBodyInertiaMoment(1000);
			Game::map->addEntity(np);
			np->mesh->r = 1.f; np->mesh->g = 1.f; np->mesh->b = 1.f;
			np->parent = this;
		}
		break;
	}
	case(2) :
	{
		vec2f dir;

		vec2f mouse = Game::getMousePos();
		glm::fvec3 unp = glm::unProject(glm::fvec3(mouse.x, mouse.y, 0), Game::cameraManager.getCurrCamera()->getViewMat(), Game::cameraManager.getCurrCamera()->getProjMat(), Game::cameraManager.getCurrCamera()->getViewport());
		dir = vec2f(unp.x - getBodyPosition().x, unp.y - getBodyPosition().y).normalise();

		auto np = new Projectile(Game::textureManager.getTexture("pmag"), 32);
		np->setPosition(getBodyPosition());
		Game::map->addEntity(np);
		np->mesh->r = 1.f; np->mesh->g = 1.f; np->mesh->b = 1.f;
		np->parent = this;
		np->setBodyMass(100);
		np->setBodyInertiaMoment(100000);
		np->applyLinearForce(dir * 1000000);
		np->lifeLeft.setSeconds(4);
		np->restitution = 0.5;
		np->pushBackStrength = 0.2;
		np->staticFriction = 0.2f;
		np->dynamicFriction = 0.2f;

		break;
	}
	case(3) :
	{
		std::vector<Physical*> rets;

		vec2f mouse = Game::getWorldMousePos();

		Game::map->eQuadTree.queryOnlyIntersecting(frect(mouse.x, mouse.y, 1, 1), rets);
		if (rets.size() > 0)
		{
			if (rets.front()->getBodyMass() == FLT_MAX) { break; }
			heldObject = rets.front();
		}
		else
		{
			heldObject = nullptr;
		}
		break;
	}
	}
}

void Player::draw(float pTimeDelta)
{
	auto shader = Game::shaderManager.getShader("minTex");

	auto t = getTransform();
	t.translate(gm);

	glow->draw(t);

	t.translate(gm.negative());
	t.translate(vec2f(0, fm));

	mesh->draw(t);

	mesh->drawConvexHull(getTransform());

	drawBounds(getTransform());

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < collisionVertsSize; i+=2)
	{
		glVertex3f(collisionVerts[i], collisionVerts[i + 1], 0.f);
	}
	glEnd();
}

void Player::saveToStream(std::ofstream& ofs)
{
	Character::saveToStream(ofs);
	glow->saveToStream(ofs);
	ofs.write((char*)&movForce, sizeof(movForce));
	ofs.write((char*)&jumpStr, sizeof(jumpStr));
	ofs.write((char*)hasPower, sizeof(hasPower));
}

void Player::loadFromStream(std::ifstream& ifs)
{
	glow = new TriangleMesh(ifs);
	ifs.read((char*)&movForce, sizeof(movForce));
	ifs.read((char*)&jumpStr, sizeof(jumpStr));
	ifs.read((char*)hasPower, sizeof(hasPower));
}
