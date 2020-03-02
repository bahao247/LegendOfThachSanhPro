#ifndef __OpponentMech_h_
#define __OpponentMech_h_

#include "Mech.h"
#include <Ogre.h>

class OpponentMech : public Mech
{
public:
    OpponentMech(Ogre::String name, Ogre::SceneManager* sceneMgr, Ogre::Real worldSize);
    virtual ~OpponentMech(void);

protected:

	double mTurnTimeSeconds;
	double mTimeToTurnSeconds;
	int detectDirection(Mech* player);
	virtual void checkBounds(const Ogre::Vector3& position);

private:	

};

#endif // #ifndef __OpponentMech_h_