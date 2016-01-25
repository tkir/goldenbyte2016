#pragma once
#include "include.h"

class Load_Singleton
{
private:
	static Load_Singleton* singleton;
	Load_Singleton();
	Load_Singleton(const Load_Singleton&);
	Load_Singleton& operator=(Load_Singleton&);


	Font font;

	SoundBuffer bufferShot;
	Sound soundShot;
	SoundBuffer bufferPlayerDamage;
	Sound soundPlayerDamage;
	SoundBuffer bufferEnemyDamage;
	Sound soundEnemyDamage;
	float soundVolume;

	Music music;

	int experienceLevelArray[5];

public:

	static Load_Singleton * getInstance() {
		if (!singleton)
			singleton = new Load_Singleton();
		return singleton;
	}

	Font* getFont(){ return &font; }

	Sound* getSoundShot(){ return &soundShot; }
	Sound* getSoundPlayerDamage(){ return &soundPlayerDamage; }
	Sound* getSoundEnemyDamage(){ return &soundEnemyDamage; }
	float getSoundVolume(){ return soundVolume; }
	void setSoundVolume(float _volume);
	Music* takeMusic();
	Music* getMusic(){ return &music; }
	int getExperienceLevel(int level){ return experienceLevelArray[level]; }
};
