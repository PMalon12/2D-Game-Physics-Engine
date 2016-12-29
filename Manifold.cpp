#pragma once
#include "Manifold.h"
#include "Physical.h"
#include "GJK.h"
#include "EPA.h"
#include "Game.h"

void ContactManifold::deleteFromObjects() const
{
	valid = false;
	auto dom = A < B ? A : B;

	auto domItr = dom->contactManifolds.find(*this);
	if (domItr != dom->contactManifolds.end())
		dom->contactManifolds.erase(domItr);
}

void ContactManifold::deleteFromOtherObject(Physical* obj) const
{
	valid = false;
	auto other = obj == A ? B : A;
	auto itr = other->contactManifolds.find(*this);
	if (itr != other->contactManifolds.end())
		other->contactManifolds.erase(itr);

	itr = other->involvedContactManifolds.find(*this);
	if (itr != other->involvedContactManifolds.end())
		other->involvedContactManifolds.erase(itr);
}

void ContactManifold::generateContacts() const
{
	//Perform AABB test on polygons
	if (!A->getGlobalBounds().intersects(B->getGlobalBounds()))
	{
		numContacts = 0;
		return;
	}

	//Perform accurate GJK intersection test
	auto gjkResult = GJK::checkCollision(*A, *B);
	if (gjkResult.first == true)
	{
		//Obtain detailed collision information
		auto epaResult = EPA::getClosestEdge(*A, *B, gjkResult.second);

		//Get vertices from both polygons
		float* aSet, *bSet;
		int aSize = A->getVertices(aSet);
		int bSize = B->getVertices(bSet);

		//More convenient names
		int aIndA = epaResult.poly.contactEdges.aEdge.ai;
		int aIndB = epaResult.poly.contactEdges.aEdge.bi;

		int bIndA = epaResult.poly.contactEdges.bEdge.ai;
		int bIndB = epaResult.poly.contactEdges.bEdge.bi;

		//Ensure order of edge indices is correct
		if (bIndB < bIndA)
		{
			int tmp = bIndA;
			bIndA = bIndB;
			bIndB = tmp;
		}

		if (aIndB < aIndA)
		{
			int tmp = aIndA;
			aIndA = aIndB;
			aIndB = tmp;
		}

		//Get egde vectors
		vec2f aEdge[2]; aEdge[0] = vec2f(aSet[aIndA], aSet[aIndA + 1]); aEdge[1] = vec2f(aSet[aIndB], aSet[aIndB + 1]);
		vec2f bEdge[2]; bEdge[0] = vec2f(bSet[bIndA], bSet[bIndA + 1]); bEdge[1] = vec2f(bSet[bIndB], bSet[bIndB + 1]);

		//Get max depth used in clipping algorithm
		vec2f aMax = aEdge[0].getTransformed(A->getTransformMat()), bMax = bEdge[0].getTransformed(B->getTransformMat());

		if (aIndA == aIndB)
		{
			//A is point, get best edge, then clip

			//Get adjacent point indices
			int lIndex = aIndA + 2 >= aSize ? 0 : aIndA + 2;
			int rIndex = aIndA - 2 < 0 ? aSize - 2 : aIndA - 2;

			//Get vectors for EPA point, and adjacent indices
			vec2f aPoint(aSet[aIndA], aSet[aIndA + 1]); ;
			vec2f lPoint(aSet[lIndex], aSet[lIndex + 1]);
			vec2f rPoint(aSet[rIndex], aSet[rIndex + 1]);
			
			//Calculate normal for left point and right point edges
			vec2f lNormal((lPoint - aPoint).getNormal().normalise());
			vec2f rNormal((aPoint - rPoint).getNormal().normalise());

			//Translate epa normal into A object space
			vec2f epNorm = epaResult.normal; epNorm.rotate(-A->getRotation());

			//Get dot products of left and right normals with the translated epa normal
			float lDot(lNormal.dot(epNorm));
			float rDot(rNormal.dot(epNorm));

			//Determine most parallel edge based on dot products just calculated
			//Set edge variables
			if (lDot > rDot)
			{
				aEdge[0] = aPoint; aEdge[1] = lPoint;
			}
			else
			{
				aEdge[0] = rPoint; aEdge[1] = aPoint;
			}
		}
		
		if (bIndA == bIndB)
		{
			//B is point, get best edge, then clip

			//Get adjacent point indices
			int lIndex = bIndA + 2 >= bSize ? 0 : bIndA + 2;
			int rIndex = bIndA - 2 < 0 ? bSize - 2 : bIndA - 2;

			//Get vectors for EPA point, and adjacent indices
			vec2f bPoint(bSet[bIndA], bSet[bIndA + 1]);
			vec2f lPoint(bSet[lIndex], bSet[lIndex + 1]);
			vec2f rPoint(bSet[rIndex], bSet[rIndex + 1]);

			//Calculate normal for left point and right point edges
			vec2f lNormal((lPoint - bPoint).getNormal().normalise());
			vec2f rNormal((bPoint - rPoint).getNormal().normalise());

			//Translate epa normal into A object space
			vec2f epNorm = epaResult.normal.negative(); epNorm.rotate(-B->getRotation());

			//Get dot products of left and right normals with the translated epa normal
			float lDot(lNormal.dot(epNorm));
			float rDot(rNormal.dot(epNorm));

			//Determine most parallel edge based on dot products just calculated
			//Set edge variables
			if (lDot > rDot)
			{
				bEdge[0] = bPoint; bEdge[1] = lPoint;
			}
			else
			{
				bEdge[0] = rPoint; bEdge[1] = bPoint;
			}
		}

		//We have edges from A and B, now clip

		vec2f* ref;
		vec2f* inc;

		//First identify reference and incident edges

		aEdge[0].transform(A->getTransformMat()); aEdge[1].transform(A->getTransformMat());
		bEdge[0].transform(B->getTransformMat()); bEdge[1].transform(B->getTransformMat());

		vec2f aEdgeVec(aEdge[1] - aEdge[0]);
		vec2f bEdgeVec(bEdge[1] - bEdge[0]);

		vec2f aNormal(aEdgeVec.getNormal());
		vec2f bNormal(bEdgeVec.getNormal());

		vec2f aSpaceEPANormal, bSpaceEPANormal;
		aSpaceEPANormal = epaResult.normal; aSpaceEPANormal.transform(A->getTransformMat()).normalise();
		bSpaceEPANormal = epaResult.normal; bSpaceEPANormal.transform(B->getTransformMat()).normalise();

		double max;

		vec2f refv, refNorm;

		//Choose reference and incident edges
		if (aNormal.normalise().dot(epaResult.normal) >= bNormal.normalise().dot(epaResult.normal.negative()))
		//if (aEdgeVec.dot(epaResult.normal) <= bEdgeVec.dot(epaResult.normal))
		{
			ref = aEdge; refv = vec2f((ref[1] - ref[0]).normalise()); refNorm = refv.getNormal();
			inc = bEdge;
			max = refNorm.dot(aMax);
		}
		else 
		{
			ref = bEdge; refv = vec2f((ref[1] - ref[0]).normalise()); refNorm = refv.getNormal();
			inc = aEdge;
			max = refNorm.dot(bMax);
		}

		//Clipping function
		auto clip = [](ClippedPoints& v, vec2f n, double o) -> void
		{
			vec2f ret[2]; int top = 0; ret[1].x = FLT_MAX;
			double d1 = n.dot(v.a) - o;
			double d2 = n.dot(v.b) - o;

			if (d1 >= 0.0) { ret[top] = v.a; top++; }
			if (d2 >= 0.0) { ret[top] = v.b; top++; }

			if (d1 * d2 < 0.0)
			{
				vec2f e = v.b - v.a;
				double u = d1 / (d1 - d2);
				e = e * u;
				e += v.a;
				ret[top] = e;
			}
			v.a = ret[0]; v.b = ret[1];
			return;
		};

		//Clip incident edge
		ClippedPoints clipped(inc[0], inc[1]);
		double clipLen1 = refv.dot(ref[0]);
		clip(clipped, refv, clipLen1); 

		//Check if clip returned valid edge
		if (clipped.b.x == FLT_MAX)
		{
			numContacts = 0;
			return;
		}

		//Clip incident edge in other direction
		double clipLen2 = -refv.dot(ref[1]);
		clip(clipped, refv.negative(), clipLen2); 

		//Check if clip returned valid edge
		if (clipped.b.x == FLT_MAX)
		{
			numContacts = 0;
			return;
		}
		
		//Calculate depths of clipped edge points
		double depth1 = refNorm.dot(clipped.a) - max;
		double depth2 = refNorm.dot(clipped.b) - max;

		//If clipped point depth is < 0 (Not inside collision manifold) then flag it as invalid
		bool a(depth1 < 0.0);
		bool b(depth2 < 0.0);

		//Determine number of contacts from the flags just set up
		//At least one will be valid as non validity of both clip points would be caught earlier
		//Set valid contact info
		if (a && b)
		{
			//TWO CONTACTS
			numContacts = 2;
			contacts[0].position = clipped.a;
			contacts[0].normal = epaResult.normal;
			contacts[0].depth = depth1;

			contacts[1].position = clipped.b;
			contacts[1].normal = epaResult.normal;
			contacts[1].depth = depth2;
		}
		else
		{
			//ONE CONTACT
			if (a)
			{
				numContacts = 1;
				contacts[0].position = clipped.a;
				contacts[0].normal = epaResult.normal;
				contacts[0].depth = depth1;
			}
			else
			{
				numContacts = 1;
				contacts[0].position = clipped.b;
				contacts[0].normal = epaResult.normal;
				contacts[0].depth = depth2;
			}
		}
	}
}

void ContactManifold::resolveCollision() const
{
	if (A->getBodyMass() == FLT_MAX && B->getBodyMass() == FLT_MAX) { return; }

	for (int i = 0; i < numContacts; ++i)
	{
		//NORMAL IMPULSE

		//Relative contact positions
		vec2d rA = contacts[i].relA, rB = contacts[i].relB;

		//Velocities
		vec2d AVelRot = rA.cross(-A->getBodyAngularVelocity());
		vec2d BVelRot = rB.cross(-B->getBodyAngularVelocity());
		vec2d AVel = A->getBodyVelocity() + AVelRot;
		vec2d BVel = B->getBodyVelocity() + BVelRot;

		//Relative velocity
		vec2d relVel = BVel - AVel;

		//Relative velocity along normal
		double velAlongNormal = relVel.dot(contacts[i].normal);

		//Calculate normal impulse magnitude
		double jr = ((velAlongNormal - contacts[i].restitutionBias) - contacts[i].penSolveMag) * -contacts[i].effectiveNormalMass;

		//Accumulate impulse
		double oldJr = contacts[i].acJr;
		contacts[i].acJr = std::max(oldJr + jr, 0.0);
		jr = contacts[i].acJr - oldJr;

		//Calculate normal impulse vector
		vec2d normalImpulse = jr * contacts[i].normal;

		//Apply normal impulse
		A->applyForce(normalImpulse.negative(), rA);
		B->applyForce(normalImpulse, rB);

		//TANGENTIAL IMPULSE

		//New velocities
		AVelRot = rA.cross(-A->getBodyAngularVelocity());
		BVelRot = rB.cross(-B->getBodyAngularVelocity());
		AVel = (A->getBodyVelocity()) + AVelRot;
		BVel = (B->getBodyVelocity()) + BVelRot;

		//New relative velocity
		relVel = BVel - AVel;

		//Relative velocity along tangent
		double velAlongTangent = relVel.dot(contacts[i].tangent);

		//Calculate tangential impulse magnitude
		double jt = -velAlongTangent * contacts[i].effectiveTangentMass;

		//Coulomb friction model
		double friction = jt < (contacts[i].staticFriction * contacts[i].acJr) ? contacts[i].staticFriction : contacts[i].dynamicFriction;

		//Accumulate impulse
		double maxJt = friction * contacts[i].acJr;
		double oldJt = contacts[i].acJt;
		contacts[i].acJt = clamp(oldJt + jt, -maxJt, maxJt);
		jt = contacts[i].acJt - oldJt;

		//Calculate tangential impulse vector
		vec2d tangentImpulse = double(jt) * contacts[i].tangent;

		//Apply tangential impulse
		A->applyForce(tangentImpulse.negative(), rA);
		B->applyForce(tangentImpulse, rB);
	}
}

void ContactManifold::setupManifold()
{
	const float allowedPen = 0.01f;

	for (int i = 0; i < numContacts; ++i)
	{
		//Get relative positions to contact
		contacts[i].relA = (contacts[i].position - A->getBodyPosition());
		contacts[i].relB = (contacts[i].position - B->getBodyPosition());

		vec2d rA = contacts[i].relA;
		vec2d rB = contacts[i].relB;

		//Get collision tangent
		contacts[i].tangent = contacts[i].normal.cross(1.f);

		//Calculate impulse required to solve interpenetration
		contacts[i].penSolveMag = (A->pushBackStrength + B->pushBackStrength) * 0.5f * std::max(-contacts[i].depth - allowedPen, 0.f) * (1.f / (Game::getDT().getSeconds()));

		//Calculate effective normal mass
		contacts[i].effectiveNormalMass = A->state.inverseMass +
											B->state.inverseMass +
											(A->state.inverseInertiaMoment * (rA.dot(rA) - std::pow(rA.dot(contacts[i].normal), int(2)))) +
											(B->state.inverseInertiaMoment * (rB.dot(rB) - std::pow(rB.dot(contacts[i].normal), int(2))));
		
		contacts[i].effectiveNormalMass = 1.0 / contacts[i].effectiveNormalMass;

		//Calculate effective tangent mass
		contacts[i].effectiveTangentMass = A->state.inverseMass +
											B->state.inverseMass +
											(A->state.inverseInertiaMoment * (rA.dot(rA) - std::pow(rA.dot(contacts[i].tangent), int(2)))) +
											(B->state.inverseInertiaMoment * (rB.dot(rB) - std::pow(rB.dot(contacts[i].tangent), int(2))));

		contacts[i].effectiveTangentMass = 1.0 / contacts[i].effectiveTangentMass;

		//Calculate static and dynamic friction coefficients
		contacts[i].staticFriction = sqrt((A->staticFriction*A->staticFriction) + (B->staticFriction*B->staticFriction));
		contacts[i].dynamicFriction = sqrt((A->dynamicFriction*A->dynamicFriction) + (B->dynamicFriction*B->dynamicFriction));


		vec2f AVelRot = rA.cross(-A->getBodyAngularVelocity());
		vec2f BVelRot = rB.cross(-B->getBodyAngularVelocity());
		vec2f AVel = A->getBodyVelocity() + AVelRot;
		vec2f BVel = B->getBodyVelocity() + BVelRot;
		vec2f relVel = BVel - AVel;

		float velNorm = contacts[i].normal.dot(BVel - AVel);

		//Calculate restitution
		double restitution;
		if (A->restitution < 0 || B->restitution < 0)
		{
			restitution = std::max((A->restitution + B->restitution) * 0.5f, 0.f);
		}
		else
		{
			restitution = std::max(A->restitution, B->restitution);
		}

		//Determine restitution bias
		if (velNorm < -0.2f)
		{
			contacts[i].restitutionBias = -restitution * velNorm;
		}
		else
		{
			contacts[i].restitutionBias = 0.f;
		}

		//Apply accumulated impulses
		vec2f impulse = (contacts[i].acJr * contacts[i].normal) + (contacts[i].acJt * contacts[i].tangent);

		A->applyForce(impulse.negative(), rA);
		B->applyForce(impulse, rB);
	}
}
