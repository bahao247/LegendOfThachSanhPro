#include "OpponentMech.h"

OpponentMech::OpponentMech(Ogre::String name, Ogre::SceneManager* sceneMgr, Ogre::Real worldSize):
	Mech(name, sceneMgr, worldSize),
	mTurnTimeSeconds(0.03f),
	mTimeToTurnSeconds(0.0f)
{
	mSpeed = 7.0f;
	mEntity->setMaterialName("OpponentMaterial");
	mMechDirection+=mMechDirection+Ogre::Math::PI/2;
	mMechNode->yaw(Ogre::Radian(Ogre::Math::PI));
	mMechNode->setPosition(-mWorldSize/4, -mEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y*mScale, -mWorldSize/4);
}

OpponentMech::~OpponentMech(void)
{
}

int OpponentMech::detectDirection(Mech* player) 
{
	Ogre::Vector3 detectDirection = player->getPosition() - mMechNode->getPosition();
	// distance = player->getPosition().distance(mMechNode->getPosition())
	Ogre::Radian angle = getDirection().angleBetween(detectDirection);
	if (angle < Ogre::Radian(Ogre::Math::PI/6)) {
		return 1;
	} else if (angle > Ogre::Radian(Ogre::Math::PI*11/6)) {
		return -1;
	}
	return 0;
}

void OpponentMech::checkBounds(const Ogre::Vector3& position)
{
	Mech::checkBounds(position);
	mSpeed = 5.0f;
}