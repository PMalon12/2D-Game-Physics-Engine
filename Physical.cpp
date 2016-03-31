#pragma once
#include "Physical.h"
#include "Map.h"
#include "Manifold.h"
#include "Game.h"
#include "Utility.cpp"

Physical::Physical(std::ifstream & ifs)
{
	collisionVerts = nullptr;
	collisionVertsSize = 0;
	density = 0.01;
	affectedByGravity = true;
	Physical::loadFromStream(ifs);
	uid.setFont(Game::fontManager.getFont("consola"));
	uid.setString(std::to_string(UID));
	uid.setCharSize(18);
	uid.setColour(Colour(1.0, 0.0, 0.0, 1.0));
	uid.setPosition(getBodyPosition() + getOrigin());
}

void Physical::updatePhysics()
{
	frect bounds = getGlobalBounds();

	int xFirst = std::max(bucketIDs.fx - 1, 0);
	int yFirst = std::max(bucketIDs.fy - 1, 0);
	int xLast = std::min(bucketIDs.lx + 1, Grid::res.x);
	int yLast = std::min(bucketIDs.ly + 1, Grid::res.y);

	involvedContactManifolds.clear();

	//GET BUCKET INDEX FOR EACH BUCKET OBJECT IS IN
	for (int x = xFirst; x < xLast; ++x)
	{
		for (int y = yFirst; y < yLast; ++y)
		{
			int i = x + (y * Grid::res.x);
			auto bucket = map->physicalBodies.buckets[i];

			//FOR EACH OBJECT IN THOSE BUCKETS
			for (auto itr = bucket.begin(); itr != bucket.end(); ++itr)
			{
				if (itr->second == this) { continue; }

				auto dom = this < itr->second ? this : itr->second;
				auto reg = this < itr->second ? itr->second : this;

				//GET CONTACT MANIFOLD FOR OBJECT
				auto newManifold = ContactManifold(this, itr->second);

				bool persistent(false);
				auto iter = dom->contactManifolds.end();

				//CHECK IF MANIFOLD ALREADY EXISTS
				bool contin = false;
				for (auto manifs = dom->contactManifolds.begin(); manifs != dom->contactManifolds.end();)
				{
					if ((*manifs) == newManifold)
					{
						if (newManifold.numContacts == 0)
						{
							newManifold.deleteFromObjects();
							newManifold.deleteFromOtherObject(this);
							contin = true;
						}
						persistent = true; //MANIFOLD ALREADY EXISTS
						iter = manifs;
						break;
					}
					++manifs;
				}
				
				if (contin == true)
				{
					continue;
				}

				if (persistent)
				{
					//COLLISION OCCURRED, MANIFOLD ALREADY EXISTS, MERGE OLD AND NEW MANIFOLDS
					iter->merge(newManifold);
					reg->involvedContactManifolds.insert(newManifold);
					involvedContactManifolds.insert(newManifold);
				}
				else
				{
					//COLLISION OCCURRED, MANIFOLD DOESNT EXIST, PUSH NEW MANIFOLD TO ONE OBJECT IN COLLISION
					if (newManifold.numContacts != 0) 
					{
						dom->contactManifolds.insert(newManifold);
						reg->involvedContactManifolds.insert(newManifold);
					}
					else
					{
						newManifold.deleteFromObjects();
						newManifold.deleteFromOtherObject(this);
					}
				}
			}
		}
	}

	for (auto itr = contactManifolds.begin(); itr != contactManifolds.end(); ++itr)
	{
		Grid::manifolds.push_back(*itr);
	}
}

void Physical::dampen(float x)
{
	applyLinearForce(state.velocity * -std::min(x, state.velocity.mag()) * Game::getDT().getSeconds());
	applyTorque(vec2f(state.angularVelocity * x, 0), vec2f(0, 50000));
}

void Physical::drawBounds(Transform& t)
{
	if (Game::getOption(DRAW_STRUCT_BOUNDS))
	{
		auto shader = Game::shaderManager.getShader("min2");
		shader->use();

		auto col = glGetUniformLocation(shader->getID(), "colour");
		glUniform4f(col, 0.7f, 0.7f, 0.f, 1.0f);

		auto model = glGetUniformLocation(shader->getID(), "model");
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(t.getGLTransform()));

		frect gBounds = getLocalBounds();

		glBegin(GL_LINE_STRIP);

		glVertex3f(gBounds.left - 1, gBounds.bot - 1, 0);
		glVertex3f(gBounds.left + gBounds.width + 1, gBounds.bot - 1, 0);
		glVertex3f(gBounds.left + gBounds.width + 1, gBounds.bot + gBounds.height + 1, 0);
		glVertex3f(gBounds.left - 1, gBounds.bot + gBounds.height + 1, 0);
		glVertex3f(gBounds.left - 1, gBounds.bot - 1, 0);

		glEnd();
	}
}

void Physical::drawID(Transform & t)
{
	if (Game::getOption(DRAW_ID))
	{

	}
}

void Physical::setUID(uint32_t pUID)
{
	UID = pUID;
	uid.setFont(Game::fontManager.getFont("consola"));
	uid.setString(std::to_string(UID));
	uid.setCharSize(18);
	uid.setColour(Colour(1.0, 0.0, 0.0, 1.0));
	uid.setPosition(getBodyPosition() + getOrigin());
}