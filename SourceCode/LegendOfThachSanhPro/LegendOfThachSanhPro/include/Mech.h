#ifndef __Mech_h_
#define __Mech_h_

#include <Ogre.h>

class Mech
{
public:
    Mech(Ogre::String name, Ogre::SceneManager* sceneMgr, Ogre::Real worldSize);
    virtual ~Mech(void);

	const Ogre::Vector3 & getPosition(void);
	Ogre::Vector3 getDirection(void);
	Ogre::Real getSpeed(void);
	void setSpeed(Ogre::Real speed);
	void accelerate(void);
	void decelerate(void);
	void turnLeft(void);
	void turnRight(void);	
	const Ogre::String getName(void);
	virtual void move(double time);
	virtual void fireLaser(Mech* opponent);
	void mechDead(void);
	bool isActive(void);
	void setDirection(Ogre::Vector3 mechDirection);
	void setPostion(Ogre::Vector3 mechPostion);
	void turnBack(void);

protected:

	Ogre::String mName;
	bool mActive, mClient;
	Ogre::Real mWorldSize;
	Ogre::Real mTurnIncrementRadians;
	Ogre::Real mMechDirection;  
	Ogre::Real mSpeed;
	Ogre::Real mSpeedChange;
	Ogre::Real mLaserLength;
	Ogre::Real mMechLength;
	Ogre::Real mScale;
	double mLaserSeconds;
	double mCurrentLaserSeconds;
	Ogre::Entity* mEntity;
	Ogre::SceneNode* mMechNode;
	Ogre::AnimationState *mAnimationState; // The current animation state of the object	
	Ogre::SceneManager* mSceneMgr;
	Ogre::RaySceneQuery *mRaySceneQuery;
	Ogre::ManualObject* mLaser; 
	Ogre::SceneNode* mLaserNode; 

	virtual bool isCollision(const Ogre::Vector3& position, const Ogre::Vector3& direction); 
	virtual void checkBounds(const Ogre::Vector3& position);
	void removeLaser(void); 
	bool laserHits(Mech* opponent);

private:	

};

#endif // #ifndef __Mech_h_