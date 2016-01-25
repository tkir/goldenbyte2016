#include "Load_Singleton.h"

Load_Singleton::Load_Singleton()
{
	srand(time(NULL));

	font.loadFromFile("files/CyrilicOld.TTF");

	bufferShot.loadFromFile("files/sound/shot.flac");
	soundShot.setBuffer(bufferShot);
	bufferPlayerDamage.loadFromFile("files/sound/playerDamage.wav");
	soundPlayerDamage.setBuffer(bufferPlayerDamage);
	bufferEnemyDamage.loadFromFile("files/sound/enemyDamage.wav");
	soundEnemyDamage.setBuffer(bufferEnemyDamage);
	setSoundVolume(100.f);

	experienceLevelArray[0] = 0;
	experienceLevelArray[1] = 3;
	experienceLevelArray[2] = 10;
	experienceLevelArray[3] = 30;
	experienceLevelArray[4] = 10000;//чтоб не вылететь за массив
}
void Load_Singleton::setSoundVolume(float _volume)
{
	soundVolume = _volume;
	soundShot.setVolume(soundVolume);
	soundPlayerDamage.setVolume(soundVolume);
	soundEnemyDamage.setVolume(soundVolume);
}
Music* Load_Singleton::takeMusic()
{
	char tmp[10]; _itoa_s(rand() % 4 + 1, tmp, 10);
	std::string str; str = "files/music/music_"; str += tmp; str += ".wav";
	music.openFromFile(str);
	return &music;
}
Load_Singleton* Load_Singleton::singleton = nullptr;