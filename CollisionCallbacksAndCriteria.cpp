#include "CollisionCallbacksAndCriteria.h"
#include "Player.h"
#include "Game.h"

void CCBAC::playerCollisionCallback(Physical* __this, Physical* collider, const ContactManifold& manifold)
{
	auto _this = reinterpret_cast<Player*>(__this);

	if (getBit(collider->getID(), PHYSICAL_BIT))
	{
		if (collider->getID() == O_SS_LAVA) { _this->damage(30 * Game::getDT().getSeconds()); _this->setJumpLock(true); return; }

		if (_this > collider) {
			if (manifold.contacts[0].normal.negative().inBetween(MAXWALKSLOPENEG, MAXWALKSLOPEPOS))
				_this->setOnGround(true);
		}
		else {
			if (manifold.contacts[0].normal.inBetween(MAXWALKSLOPENEG, MAXWALKSLOPEPOS))
				_this->setOnGround(true);
		}
	}
}

bool CCBAC::playerCollisionCriteria(Physical* __this, const Physical* collider, const ContactManifold& manifold)
{
	return true;
}