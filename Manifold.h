#pragma once
#include "Include.h"

class Physical;

class Contact
{
public:
	Contact() : acJr(0.f), acJt(0.f), acJp(0.f) {}
	~Contact() {}

	int edgeNum;
	vec2f position;
	vec2f relA, relB;
	vec2f normal;
	vec2f tangent;
	float depth;
	float acJr, acJt, acJp;
	float effectiveNormalMass, effectiveTangentMass;
	float penSolveMag;
	float staticFriction;
	float dynamicFriction;
	float restitutionBias;
};

class ContactManifold
{
public:
	ContactManifold() {}
	ContactManifold(Physical* pA, Physical* pB)
	{
		valid = true;
		numContacts = 0;
		assert(pA != pB);
		if (pA < pB)
		{
			A = pA; B = pB;
		}
		else
		{
			A = pB; B = pA;
		}

		generateContacts();
	}
	~ContactManifold() 
	{
	}

	struct ClippedPoints
	{
		ClippedPoints(vec2f pa, vec2f pb) : a(pa), b(pb) {}
		vec2f a, b;
	};

	void deleteFromObjects() const;
	void deleteFromOtherObject(Physical* obj) const;

	Physical* A, *B;
	mutable bool valid;

	mutable Contact contacts[2];
	mutable int numContacts;

	mutable float combinedFriction;

	void resolveCollision() const;
	void setupManifold();

	void generateContacts() const;

	void merge(ContactManifold& newManifold) const
	{
		for (int i = 0; i < newManifold.numContacts; ++i)
		{
			int merge = -1;
			for (int j = 0; j < numContacts; ++j)
			{
				//Check distance between contacts
				merge = j;
			}

			if (merge > -1)
			{
				auto oldContact = contacts[i];
				contacts[i] = newManifold.contacts[i];
				contacts[i].acJr = oldContact.acJr;
				contacts[i].acJp = oldContact.acJp;
				contacts[i].acJt = oldContact.acJt;
			}
			else
			{
				contacts[i] = newManifold.contacts[i];
			}
		}

		numContacts = newManifold.numContacts;
	}

	bool operator <(const ContactManifold& rhs) const
	{
		//return ((A < rhs.A) || (A == rhs.A && B == rhs.B));
		if (A < rhs.A)
		{
			return true;
		}

		if (A == rhs.A && B < rhs.B)
		{
			return true;
		}
		return false;
	}

	bool operator ==(const ContactManifold& rhs) const
	{
		return (A == rhs.A && B == rhs.B);
	}

};