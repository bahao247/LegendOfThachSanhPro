#include <SoundManager.h>


SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::initSound()
{
	m_engine = irrklang::createIrrKlangDevice();
}

void SoundManager::deleteSound()
{
	m_engine->drop();
}

void SoundManager::setBackgroundMusic(const char* soundFileName)
{
	m_engine->play2D(soundFileName, true);
}

void SoundManager::setBackgroundMusicVolume(const float value)
{
	m_BackgroundMusic->setVolume(value);
}

void SoundManager::removeBackroundMusic()
{
	m_BackgroundMusic->drop();
}

void SoundManager::playSound(const char* soundFileName, Ogre::Vector3 pos, bool isLoop)
{
	m_engine->play3D(soundFileName, irrklang::vec3df(pos.x, pos.y, pos.z), isLoop);
}

void SoundManager::updateListenerPos(Ogre::Vector3 pos)
{
	m_engine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z), irrklang::vec3df(0,0,0));
}