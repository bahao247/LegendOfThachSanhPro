#include "Mech.h"

//-------------------------------------------------------------------------------------
Mech::Mech(Ogre::String name, Ogre::SceneManager* sceneMgr, Ogre::Real worldSize):
	mActive(true),
	mSpeed(0.0f),
	mMechDirection(Ogre::Math::PI/2),
	mTurnIncrementRadians(Ogre::Math::PI/250),
	mWorldSize(worldSize),
	mScale(0.8f),
	mName(name),
	mSceneMgr(sceneMgr),
	mLaserSeconds(0.05f),
	mCurrentLaserSeconds(0.0f),
	mLaserLength(50.0f),
	mSpeedChange(0.00001f)
{
	mEntity = mSceneMgr->createEntity(name, "Mech.mesh");
	Ogre::AxisAlignedBox box = mEntity->getBoundingBox();
    mMechNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mMechNode->attachObject(mEntity);
	mMechNode->setScale(mScale, mScale, mScale);
	mMechLength = box.getSize().x;

	mMechNode->setPosition(mWorldSize/4, -box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y*mScale, mWorldSize/4);
	mEntity->setCastShadows(true);
	mAnimationState = mEntity->getAnimationState("Walkcycle");
	mAnimationState->setLoop(true);
	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());

	mLaser =  mSceneMgr->createManualObject("laser"+name); 
	mLaserNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("laser_node"+name); 
}

//-------------------------------------------------------------------------------------
Mech::~Mech(void)
{
	mSceneMgr->destroyQuery(mRaySceneQuery);
}

bool Mech::isActive(void) 
{
	return mActive;
}

Ogre::Real Mech::getSpeed(void) 
{
	return mSpeed;
}

const Ogre::Vector3 & Mech::getPosition(void) 
{
	return mMechNode->getPosition();
}

Ogre::Vector3 Mech::getDirection(void) 
{
	return Ogre::Vector3(Ogre::Math::Cos(mMechDirection),0,Ogre::Math::Sin(mMechDirection));
}

void Mech::accelerate(void)
{
	if (mSpeed<10 && mActive) mSpeed+=mSpeedChange;
}

void Mech::decelerate(void)
{
	if (mSpeed>0 && mActive) mSpeed-=mSpeedChange;
}

void Mech::turnLeft(void) 
{
	if (mSpeed>0 && mActive) {
		mMechDirection -= mTurnIncrementRadians;
		mMechNode->yaw(Ogre::Radian(mTurnIncrementRadians));
	}
}

void Mech::turnRight(void)
{
	if (mSpeed>0 && mActive) {
		mMechDirection += mTurnIncrementRadians;
		mMechNode->yaw(Ogre::Radian(-mTurnIncrementRadians));
	}
}

void Mech::removeLaser(void) 
{
	mLaser->clear();
	mLaserNode->detachObject(mLaser);
}

void Mech::fireLaser(Mech* opponent) 
{
	if (mCurrentLaserSeconds<=0 && mActive) {
		Ogre::Vector3 mechHeight = Ogre::Vector3(0, mEntity->getBoundingBox().getSize().y*0.5, 0);
		mLaser->begin("LaserMaterial", Ogre::RenderOperation::OT_LINE_LIST); 
		mLaser->position(mechHeight+getPosition()+getDirection()*mMechLength/2); 
		mLaser->position(mechHeight+getPosition()+getDirection()*(mLaserLength+mMechLength/2));  
		mLaser->end(); 
		mLaserNode->attachObject(mLaser);
		mCurrentLaserSeconds = mLaserSeconds;
		if (laserHits(opponent)) {
			opponent->mechDead();
		}
	}
}

bool Mech::laserHits(Mech* opponent) 
{
	Ogre::Ray ray(getPosition(), getDirection());
    mRaySceneQuery->setRay(ray);
	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
	for (itr = result.begin(); itr != result.end(); itr++) {
		if (itr->movable->getName().compare(opponent->getName())==0) {
			return itr->distance<(mLaserLength+mMechLength/2);
		}
	}
	return false;
}

void Mech::mechDead(void)
{
	if (mActive) {
		mActive=false;
		mMechNode->translate(0.0f, mEntity->getBoundingBox().getHalfSize().y*mScale, 0.0f);
		mMechNode->pitch(Ogre::Radian(Ogre::Math::PI));
		mSpeed=0;
	}
}

const Ogre::String Mech::getName(void) 
{
	return mName;
}

void Mech::move(double time) 
{	
	if (mCurrentLaserSeconds>0) {
		mCurrentLaserSeconds-=time;
		if (mCurrentLaserSeconds<=0) {
			removeLaser();
		}
	}
	if (!mActive) {
		return;
	}
	if (mSpeed>0) {
		mAnimationState->setEnabled(true);
		mAnimationState->addTime(time * mSpeed/5);
		Ogre::Vector3 position = mMechNode->getPosition();
		if (!isCollision(position, getDirection())) {
			mMechNode->translate(getDirection() * mSpeed * time * 2.5);
			checkBounds(position);
		}
	} else {
		mAnimationState->setEnabled(false);
	}
}

bool Mech::isCollision(const Ogre::Vector3& position, const Ogre::Vector3& direction) 
{
	Ogre::Ray ray(position, direction);
    mRaySceneQuery->setRay(ray);
	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
	for (itr = result.begin(); itr != result.end(); itr++) {
		if (itr->movable->getName().compare(mName)!=0 && itr->distance<mMechLength/2) {
				return true;
		}
	}
	return false;
}

void Mech::checkBounds(const Ogre::Vector3& position)
{
	Ogre::Vector3 newPosition(position.x, position.y, position.z);
	bool changed = false;
	if (position.x>mWorldSize/2){
		newPosition.x = mWorldSize/2;
		changed = true;
	} else if (position.x<-mWorldSize/2) {
		newPosition.x = -mWorldSize/2;
		changed = true;
	}

	if (position.z>mWorldSize/2){
		newPosition.z = mWorldSize/2;
		changed = true;
	} else if (position.z<-mWorldSize/2) {
		newPosition.z = -mWorldSize/2;
		changed = true;
	}

	if (changed) {
		mMechNode->setPosition(newPosition);
		mAnimationState->setEnabled(false);
		mMechNode->yaw(Ogre::Radian(Ogre::Math::PI));
		mMechDirection = fmod(mMechDirection+Ogre::Math::PI, 2*Ogre::Math::PI);
		mSpeed = 0;
	}
}