#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <irrklang.h>
#include <Ogre.h>

class SoundManager
{
protected:

	irrklang::ISoundEngine*		m_engine;
	irrklang::ISound*			m_BackgroundMusic;

public:

	SoundManager();
	virtual ~SoundManager();

	virtual void initSound();
	virtual void deleteSound();

	virtual void setBackgroundMusic(const char*);
	virtual void setBackgroundMusicVolume(const float);
	virtual void removeBackroundMusic();

	virtual void playSound(const char* , Ogre::Vector3, bool isLoop = false);
	virtual void updateListenerPos(Ogre::Vector3 );
};

#endif