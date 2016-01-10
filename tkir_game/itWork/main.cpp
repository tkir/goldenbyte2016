#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <tmx/MapLoader.h>
#include <iostream>
#include <vector>
#include <list>
using namespace sf;

enum skillType{ otherType, noImage, musicOn, musicOff, webThrow, strength, silence, horror, gainArmor, sword, lance, fireBall, fireBurst, purpleBall };

class Entity;
class GameView;
class SF_Desk;
class SFGUI_Entity;
class Player;
class Skill;
class SFGUI_Window;
class FireBall;
class Footer;
class MainMenu;
class OptionMenu;

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

struct Properties
{
	int experience = 0,
		level = 0,
		health = 0;
	float speed = 0.f,
		visionRange = 0.f,
		experienceProgress = 0.f;
	void setUpExperience(int _upExperience)
	{
		experience += _upExperience;
		if (experience > Load_Singleton::getInstance()->getExperienceLevel(level + 1))level++;
		experienceProgress = ((float)experience -
			(float)Load_Singleton::getInstance()->getExperienceLevel(level)) /
			((float)Load_Singleton::getInstance()->getExperienceLevel(level + 1) -
			(float)Load_Singleton::getInstance()->getExperienceLevel(level));
	}

	struct Inventory
	{

	}inventory;

	struct Dress
	{
		struct LeftHand
		{
			int damageMin = 0,
				damageMax = 0,
				attackMin = 0,
				attackMax = 0,
				armor = 0,
				criticalChance = 0;
		}leftHand;
		struct RightHand
		{
			int damageMin = 0,
				damageMax = 0,
				attackMin = 0,
				attackMax = 0,
				armor = 0,
				criticalChance = 0;
		}rightHand;
		struct Chest
		{
			int damageMin = 0,
				damageMax = 0,
				attackMin = 0,
				attackMax = 0,
				armor = 0,
				criticalChance = 0;
		}chest;

		int countDamage()
		{
			int normal = 0;
			normal += (leftHand.damageMax - leftHand.damageMin) ? rand() % (leftHand.damageMax - leftHand.damageMin) + leftHand.damageMin : leftHand.damageMin;
			normal += (rightHand.damageMax - rightHand.damageMin) ? rand() % (rightHand.damageMax - rightHand.damageMin) + rightHand.damageMin : rightHand.damageMin;
			normal += (chest.damageMax - chest.damageMin) ? rand() % (chest.damageMax - chest.damageMin) + chest.damageMin : chest.damageMin;
			int critical =
				(rand() % 100 * (leftHand.criticalChance + rightHand.criticalChance + chest.criticalChance) >= 100) ?
				(leftHand.damageMax + rightHand.damageMax + chest.damageMax) : 0;
			return normal + critical;
		}
	}dress;

	struct Skills
	{
	private:
		struct SkillStruct
		{
			skillType type;
			int level;
			SkillStruct(skillType _type, int _level) :type(_type), level(_level){}
		};
	public:
		std::vector<std::shared_ptr<SkillStruct>> skillLevel;
		Skills()
		{
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::webThrow, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::strength, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::silence, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::horror, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::gainArmor, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::sword, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::lance, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::fireBall, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::fireBurst, 0));
			skillLevel.push_back(std::make_shared<SkillStruct>(skillType::purpleBall, 0));
		}
		void setSkillLevel(skillType _type, int _level)
		{
			for (auto it = skillLevel.begin(); it != skillLevel.end(); ++it)
			{
				if ((*it)->type == _type)
				{
					(*it)->level = _level;
					break;
				}
			}
		}
		int getSkillLevel(skillType _type)
		{
			for (auto it = skillLevel.begin(); it != skillLevel.end(); ++it)
			{
				if ((*it)->type == _type)
				{
					return (*it)->level;
				}
			}
		}
	}skills;

	struct Fast
	{
		skillType fastSkill[7];
		int fastLoad[7];	//общее время перезагрузки
		int fastLoading[7];	//прошло времени с перезагрузки

	}fast;

	struct Charakter
	{
		int strength = 0,
			dexterity = 0,
			vitality = 0,
			energy = 0;
	}charakter;

	int getDamage()
	{
		int tmpDamage = dress.countDamage();
		tmpDamage += tmpDamage * charakter.strength / 100;

		return tmpDamage;
	}
	int getArmor()
	{
		int tmpArmor = dress.leftHand.armor + dress.rightHand.armor + dress.chest.armor;
		tmpArmor += tmpArmor*charakter.dexterity / 100;

		return tmpArmor;
	}
};

class Entity
{
public:
	enum entityType{ otherEntity, player, enemy, bullet, lettering, picture, pausePicture, sfguiButton, skillEntity };
protected:
	static unsigned long IDStatic;
	enum { stay, right, left, up, down, leftUp, rightUp, rightDown, leftDown, hitUp, hitDown, hitLeft, hitRight }direction;
	entityType type;
	skillType typeSkill;
	Properties prop;
	Vector2f
		position,
		dPos,			//изменение положени¤
		targetPosition,
		targetVector;
	float width,	    //ширина
		height,			//высота
		curentFrame,
		moveTimer,
		targetDistance,
		pauseBeforHit,
		updateTime;
	int id,
		shotAuthorID,
		persecutionID;
	bool isLife,
		isHit;
	String name,
		imageName,
		str;
	Image image;
	Texture texture;
	Sprite sprite;
	GameView* view;
	Text text;
	std::vector<tmx::MapObject>* collisionObj;

public:
	Entity(String = "", String _name = "", std::vector<tmx::MapObject>* _collisionObj = nullptr, Vector2f = { 0.f, 0.f }, entityType = otherEntity);
	~Entity(){ --IDStatic; }

	entityType getType(){ return type; }
	Properties& getProperties(){ return prop; }
	Sprite getSprite(){ return sprite; }
	Text getText(){ return text; }
	Vector2f getPosition(){ return position; }
	String getName(){ return name; }
	bool getLife(){ return isLife; }
	void setHealth(float _health){}
	void setPosition(Vector2f _position){ position = _position; }
	void setTargetPosition(Vector2f _targetPosition);
	int getID(){ return id; }
	void setPersecution(int _persecutionID){ persecutionID = _persecutionID; }
	int getPersecution(){ return persecutionID; }
	FloatRect getRect(){ return FloatRect(position.x, position.y, width, height); }

	virtual void update(float) = 0;
	virtual void setInjury(int _injury, int _shotAuthorID) = 0;
};
Entity::Entity(String _image, String _name, std::vector<tmx::MapObject>* _collisionObj, Vector2f _position, entityType _type)
{
	++IDStatic;
	type = _type;
	typeSkill = skillType::otherType;
	name = _name;
	imageName = _image;
	position = _position;
	dPos = { 0, 0 };
	isLife = true;
	isHit = false;
	str = _image;
	width = 0.f;
	height = 0.f;
	id = IDStatic;
	view = nullptr;
	curentFrame = 0;
	pauseBeforHit = 0;
	updateTime = 0;
	persecutionID = 0;
	//lvl = _collisionObj;
	collisionObj = _collisionObj;
	if (type != lettering && imageName != "")
	{
		image.loadFromFile(imageName);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(width / 2, height / 2);
	}
}
void Entity::setTargetPosition(Vector2f _targetPosition)
{
	Vector2f _dif = _targetPosition - position;
	targetVector = _dif / sqrt(_dif.x*_dif.x + _dif.y*_dif.y);
	targetPosition = _targetPosition;
}
unsigned long Entity::IDStatic = 0;

class GameView
{
private:
	View view;
	Entity* player;
	Vector2f viewCenter;
public:
	GameView(Entity* = nullptr, Vector2f = { 0.f, 0.f });
	View getView(){ return view; }
	void update();
};
GameView::GameView(Entity* _player, Vector2f _viewCenter)
{
	player = _player;
	view.reset(FloatRect(0, 0, 1024, 768));
	viewCenter = _viewCenter;
}
void GameView::update()
{
	viewCenter.x = player->getPosition().x + 250;
	viewCenter.y = player->getPosition().y;
	/*if (player->getPosition().x < 262.0f) viewCenter.x = 512.0f;
	if (player->getPosition().y < 384.0f) viewCenter.y = 384.0f;*/

	view.setCenter(viewCenter);
}

class Cursor
{
public:
	enum cursorType{ stdCur, swordCur, arrowCur, handCur, skillCur };
private:
	cursorType typeCur,
		typeCurBase;
	String imageName;
	Image image;
	Texture texture;
	Sprite sprite;
	Vector2f position;
	std::list <Entity*>* entities;

	void switchCur();
public:
	Cursor(std::list <Entity*>* = nullptr);
	Sprite getSprite(){ return sprite; }
	void update(Vector2f _position);
	void setCursor(cursorType _typeCur = stdCur);
};
Cursor::Cursor(std::list <Entity*>* _entities)
{
	entities = _entities;
	imageName = "images/cursor.png";
	image.loadFromFile(imageName);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	typeCur = typeCurBase = stdCur;
	switchCur();
	update({ 0.f, 0.f });
}
void Cursor::switchCur()
{
	switch (typeCur)
	{
	case stdCur:
		sprite.setTextureRect(IntRect(6, 109, 21, 21));
		break;
	case swordCur:
		sprite.setTextureRect(IntRect(267, 11, 21, 21));
		break;
	case arrowCur:
		sprite.setTextureRect(IntRect(355, 108, 21, 21));
		break;
	case handCur:
		sprite.setTextureRect(IntRect(4, 302, 21, 21));
		break;
	case skillCur:
		sprite.setTextureRect(IntRect(92, 205, 21, 21));
		break;
	default:
		break;
	}
}
void Cursor::update(Vector2f _position)
{
	position = _position;
	sprite.setPosition(position);
	for (auto it : *entities)
	{
		if (it->getRect().contains(position) &&
			it->getType() == Entity::entityType::enemy)
		{
			setCursor((typeCurBase == stdCur) ? swordCur : typeCurBase);
			break;
		}
		else setCursor(typeCurBase);
	}

}
void Cursor::setCursor(cursorType _typeCur)
{
	if (typeCur != _typeCur)
	{
		typeCurBase = (_typeCur == skillCur) ? _typeCur : stdCur;
		typeCur = _typeCur;
		switchCur();
	}
}

class Lettering : public Entity
{
public:
	Lettering(String _str, String _name, Vector2f _position);
	void update(float);
	void setInjury(int, int){}
};
Lettering::Lettering(String str, String name, Vector2f position) : Entity(str, name, nullptr, position, lettering)
{
	type = lettering;
	if (name == "lifeDown")
	{
		text.setString(str);
		text.setFont(*Load_Singleton::getInstance()->getFont());
		text.setCharacterSize(20);
		text.setColor(Color::Red);
	}
}
void Lettering::update(float time)
{
	updateTime = time;
	dPos.y += updateTime*0.015f;
	if (name == "lifeDown")
	{
		if (dPos.y > 15)isLife = false;
		text.setPosition(position.x, position.y - dPos.y);
	}
}

class FireBall : public Entity
{
private:
	std::list <Entity*>* entities;

	void checkCollisionMap();
	void checkCollisionEntity();
	void setInjury(int, int){}
public:
	FireBall(String = "", String = "", int = 0, std::vector<tmx::MapObject>* _collisionObj = nullptr, std::list <Entity*>* _entities = nullptr, int _shotAuthorID = 0, Vector2f _target = { 0.f, 0.f });

	void update(float _time);
};
FireBall::FireBall(String, String, int, std::vector<tmx::MapObject>* _collisionObj, std::list <Entity*>* _entities, int _shotAuthorID, Vector2f _target) :Entity("", "", _collisionObj)
{
	int arrDamageMax[20] = { 0, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 50 };
	int arrDamageMin[20] = { 0, 4, 5, 6, 8, 10, 11, 13, 15, 16, 18, 20, 22, 23, 25, 27, 29, 30, 32, 40 };
	int arrRegeneration[20] = { 0, 2000, 3000, 3000, 3000, 3000, 3000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 5000, 5000, 5000, 5000, 5000 };

	type = skillEntity;
	typeSkill = skillType::fireBall;
	entities = _entities;
	shotAuthorID = _shotAuthorID;
	targetPosition = _target;
	width = height = 8.f;
	prop.speed = 0.25f;	

	image.loadFromFile("images/skill/fireBallLitle.png");
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setOrigin(width / 2, height / 2);
	for (auto it : *entities)
	{
		if (it->getID() == shotAuthorID)
		{
			position = it->getPosition();
			prop.level = it->getProperties().skills.getSkillLevel(typeSkill);
			for (int i = 0; i < 7; ++i)
			{
				if (it->getProperties().fast.fastSkill[i] == typeSkill)
				{
					it->getProperties().fast.fastLoad[i] = arrRegeneration[prop.level];
					break;
				}
			}
			break;
		}
	}

	Vector2f _dif = targetPosition - position;
	dPos = _dif / sqrt(_dif.x*_dif.x + _dif.y*_dif.y);

	prop.dress.rightHand.damageMax = arrDamageMax[prop.level];
	prop.dress.rightHand.damageMin = arrDamageMin[prop.level];
}
void FireBall::update(float _time)
{
	updateTime = _time;
	position += dPos * prop.speed * updateTime;
	checkCollisionMap();
	checkCollisionEntity();
	sprite.setPosition(position.x + width / 2.f, position.y + height / 2.f);
}
void FireBall::checkCollisionMap()
{
	for (auto it : *collisionObj)
	{
		if (getRect().intersects(it.GetAABB()))
		{
			isLife = false;
			break;
		}
	}
}
void FireBall::checkCollisionEntity()
{
	for (auto it : *entities)
	{
		if (it->getType() == enemy && getRect().intersects(it->getRect()))
		{
			isLife = false;
			it->setInjury(prop.getDamage(), shotAuthorID);
			break;
		}
	}
}

class FireBurst : public Entity
{
private:
	float radius;
	bool isShot;
	std::list <Entity*>* entities;

	void switchDirection();
	void checkCollisionMap();
	void checkCollisionEntity();
	void setInjury(int, int){}
public:
	FireBurst(String = "", String = "", int = 0, std::vector<tmx::MapObject>* _collisionObj = nullptr, std::list <Entity*>* _entities = nullptr, int _shotAuthorID = 0, Vector2f _target = { 0.f, 0.f });

	void update(float _time);
};
FireBurst::FireBurst(String, String, int, std::vector<tmx::MapObject>* _collisionObj, std::list <Entity*>* _entities, int _shotAuthorID, Vector2f _target) :Entity("", "", _collisionObj)
{
	int arrDamageMax[20] = { 0, 20, 24, 28, 32, 36, 40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 120, 140, 180 };
	int arrDamageMin[20] = { 0, 10, 12, 14, 16, 18, 20, 23, 26, 30, 34, 38, 45, 50, 55, 60, 75, 90, 110, 130 };
	int arrRadius[20] = { 0, 180, 30, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 102, 104, 106, 130 };
	int arrRegeneration[20] = { 0, 8000, 8000, 8000, 8000, 8000, 9000, 9000, 9000, 9000, 9000, 10000, 10000, 10000, 10000, 10000, 10000, 11000, 11000, 11000 };

	type = skillEntity;
	typeSkill = skillType::fireBurst;
	isShot = false;
	entities = _entities;
	shotAuthorID = _shotAuthorID;
	targetPosition = _target;
	width = 15.f;
	height = 15.f;
	prop.speed = 0.15f;
	direction = left;

	image.loadFromFile("images/skill/fireBurst.png");
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setOrigin(width / 2, height / 2);
	sprite.setScale(0.25f, 0.25f);

	for (auto it : *entities)
	{
		if (it->getID() == shotAuthorID)
		{
			position.x = it->getRect().left;
			position.y = it->getRect().top;
			prop.level = it->getProperties().skills.getSkillLevel(typeSkill);
			for (int i = 0; i < 7; ++i)
			{
				if (it->getProperties().fast.fastSkill[i] == typeSkill)
				{
					it->getProperties().fast.fastLoad[i] = arrRegeneration[prop.level];
					break;
				}
			}
			break;
		}
	}

	Vector2f _dif = targetPosition - position;
	dPos = _dif / sqrt(_dif.x*_dif.x + _dif.y*_dif.y);
	sprite.rotate(atan2f(_dif.y, _dif.x)*180.f / 3.1415926f);

	prop.dress.rightHand.damageMax = arrDamageMax[prop.level];
	prop.dress.rightHand.damageMin = arrDamageMin[prop.level];
	radius = arrRadius[prop.level];
}
void FireBurst::update(float _time)
{
	updateTime = _time;
	position += dPos * prop.speed * updateTime;
	if (direction == left)
		checkCollisionMap();
	checkCollisionEntity();
	switchDirection();
	sprite.setPosition(position.x, position.y);
	if ((int)curentFrame >= 24)isLife = false;
}
void FireBurst::switchDirection()
{
	switch (direction)
	{
	case left:
	{
		curentFrame += 0.015f*updateTime;
		if (curentFrame > 5.f)curentFrame = 0.f;
		if ((int)curentFrame == 0){ sprite.setTextureRect(IntRect(0, 828, 60, 26)); }
		else if ((int)curentFrame == 1){ sprite.setTextureRect(IntRect(61, 828, 60, 26)); }
		else if ((int)curentFrame == 2){ sprite.setTextureRect(IntRect(124, 828, 60, 26)); }
		else if ((int)curentFrame == 3){ sprite.setTextureRect(IntRect(191, 828, 60, 26)); }
		else if ((int)curentFrame == 4){ sprite.setTextureRect(IntRect(256, 828, 60, 26)); }
		break;
	}
	case hitLeft:
	{
		curentFrame += 0.005f*updateTime;
		if ((int)curentFrame == 8){ sprite.setTextureRect(IntRect(0, 0, 386, 206)); }
		else if ((int)curentFrame == 9){ sprite.setTextureRect(IntRect(387, 0, 386, 206)); }
		else if ((int)curentFrame == 10){ sprite.setTextureRect(IntRect(774, 0, 386, 206)); }
		else if ((int)curentFrame == 11){ sprite.setTextureRect(IntRect(1161, 0, 386, 206)); }
		else if ((int)curentFrame == 12){ sprite.setTextureRect(IntRect(0, 207, 386, 206)); }
		else if ((int)curentFrame == 13){ sprite.setTextureRect(IntRect(387, 207, 386, 206)); }
		else if ((int)curentFrame == 14){ sprite.setTextureRect(IntRect(774, 207, 386, 206)); }
		else if ((int)curentFrame == 15){ sprite.setTextureRect(IntRect(1161, 207, 386, 206)); }
		else if ((int)curentFrame == 16){ sprite.setTextureRect(IntRect(0, 414, 386, 206)); }
		else if ((int)curentFrame == 17){ sprite.setTextureRect(IntRect(387, 414, 386, 206)); }
		else if ((int)curentFrame == 18){ sprite.setTextureRect(IntRect(774, 414, 386, 206)); }
		else if ((int)curentFrame == 19){ sprite.setTextureRect(IntRect(1161, 414, 386, 206)); }
		else if ((int)curentFrame == 20){ sprite.setTextureRect(IntRect(0, 621, 386, 206)); }
		else if ((int)curentFrame == 21){ sprite.setTextureRect(IntRect(387, 621, 386, 206)); }
		else if ((int)curentFrame == 22){ sprite.setTextureRect(IntRect(774, 621, 386, 206)); }
		else if ((int)curentFrame == 23){ sprite.setTextureRect(IntRect(1161, 621, 386, 206)); }
		break;
	}
	default:
		break;
	}
}
void FireBurst::checkCollisionMap()
{
	for (auto it : *collisionObj)
	{
		if (getRect().intersects(it.GetAABB()))
		{
			isLife = false;
			break;
		}
	}
}
void FireBurst::checkCollisionEntity()
{
	for (auto it : *entities)
	{
		if (it->getType() == enemy && getRect().intersects(it->getRect()))
		{
			if (isShot)
				it->setInjury(prop.getDamage(), shotAuthorID);
			if (curentFrame < 8.f)
			{
				direction = hitLeft;
				curentFrame = 8.f;
				dPos = { 0.f, 0.f };
				sprite.setScale(0.003f*radius, 0.003*radius);
				sprite.setRotation(0.f);
				width = height = radius*2.f;
				sprite.setOrigin(width / 2.f, height / 2.f);
				isShot = true;
				it = *entities->begin();
			}
		}
	}
	isShot = false;
}

class PurpleBall : public Entity
{
private:
	std::list <Entity*>* entities;

	void switchDirection();
	void checkCollisionMap();
	void checkCollisionEntity();
	void setInjury(int, int){}
public:
	PurpleBall(String = "", String = "", int = 0, std::vector<tmx::MapObject>* _collisionObj = nullptr, std::list <Entity*>* _entities = nullptr, int _shotAuthorID = 0, Vector2f _target = { 0.f, 0.f });

	void update(float _time);
};
PurpleBall::PurpleBall(String, String, int, std::vector<tmx::MapObject>* _collisionObj, std::list <Entity*>* _entities, int _shotAuthorID, Vector2f _target) :Entity("", "", _collisionObj)
{
	int arrDamageMax[20] = { 0, 20, 24, 28, 32, 36, 40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 120, 140, 180 };
	int arrDamageMin[20] = { 0, 10, 12, 14, 16, 18, 20, 23, 26, 30, 34, 38, 45, 50, 55, 60, 75, 90, 110, 130 };
	int arrRegeneration[20] = { 0, 8000, 8000, 8000, 8000, 8000, 9000, 9000, 9000, 9000, 9000, 10000, 10000, 10000, 10000, 10000, 10000, 11000, 11000, 11000 };

	type = skillEntity;
	typeSkill = skillType::purpleBall;
	entities = _entities;
	shotAuthorID = _shotAuthorID;
	targetPosition = _target;
	width = 15.f;
	height = 15.f;
	prop.speed = 0.15f;
	direction = left;

	image.loadFromFile("images/skill/purpleBall.png");
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setOrigin(width / 2, height / 2);
	sprite.setScale(0.25f, 0.25f);

	for (auto it : *entities)
	{
		if (it->getID() == shotAuthorID)
		{
			position.x = it->getRect().left;
			position.y = it->getRect().top;
			prop.level = it->getProperties().skills.getSkillLevel(typeSkill);
			for (int i = 0; i < 7; ++i)
			{
				if (it->getProperties().fast.fastSkill[i] == typeSkill)
				{
					it->getProperties().fast.fastLoad[i] = arrRegeneration[prop.level];
					break;
				}
			}
			break;
		}
	}

	Vector2f _dif = targetPosition - position;
	dPos = _dif / sqrt(_dif.x*_dif.x + _dif.y*_dif.y);
	sprite.rotate(atan2f(_dif.y, _dif.x)*180.f / 3.1415926f - 180.f);

	prop.dress.rightHand.damageMax = arrDamageMax[prop.level];
	prop.dress.rightHand.damageMin = arrDamageMin[prop.level];
}
void PurpleBall::update(float _time)
{
	updateTime = _time;
	position += dPos * prop.speed * updateTime;
	checkCollisionMap();
	checkCollisionEntity();
	switchDirection();
	sprite.setPosition(position.x + width / 2.f, position.y + height / 2.f);
	if (curentFrame > 13.9f) isLife = false;
}
void PurpleBall::switchDirection()
{
	switch (direction)
	{
	case left:
	{
		curentFrame += 0.015f*updateTime;
		if (curentFrame > 8.f)curentFrame = 0.f;
		if ((int)curentFrame == 0){ sprite.setTextureRect(IntRect(0, 0, 91, 86)); }
		else if ((int)curentFrame == 1){ sprite.setTextureRect(IntRect(92, 0, 130, 86)); }
		else if ((int)curentFrame == 2){ sprite.setTextureRect(IntRect(222, 0, 87, 86)); }
		else if ((int)curentFrame == 3){ sprite.setTextureRect(IntRect(309, 0, 152, 86)); }
		else if ((int)curentFrame == 4){ sprite.setTextureRect(IntRect(461, 0, 111, 86)); }
		else if ((int)curentFrame == 5){ sprite.setTextureRect(IntRect(572, 0, 150, 86)); }
		else if ((int)curentFrame == 6){ sprite.setTextureRect(IntRect(722, 0, 120, 86)); }
		else if ((int)curentFrame == 7){ sprite.setTextureRect(IntRect(842, 0, 146, 86)); }
		break;
	}
	case hitLeft:
	{
		curentFrame += 0.01f*updateTime;
		if ((int)curentFrame == 8){ sprite.setTextureRect(IntRect(0, 98, 90, 120)); }
		else if ((int)curentFrame == 9){ sprite.setTextureRect(IntRect(90, 98, 92, 120)); }
		else if ((int)curentFrame == 10){ sprite.setTextureRect(IntRect(163, 98, 73, 120)); }
		else if ((int)curentFrame == 11){ sprite.setTextureRect(IntRect(237, 98, 81, 120)); }
		else if ((int)curentFrame == 12){ sprite.setTextureRect(IntRect(318, 98, 66, 120)); }
		else if ((int)curentFrame == 13){ sprite.setTextureRect(IntRect(384, 98, 39, 120)); }
		break;
	}
	default:
		break;
	}
}
void PurpleBall::checkCollisionMap()
{
	for (auto it : *collisionObj)
	{
		if (getRect().intersects(it.GetAABB()))
		{
			if (curentFrame < 8.f)
			{
				direction = hitLeft;
				curentFrame = 8.f;
				dPos = { 0.f, 0.f };
				prop.speed = 0.f;
			}
			break;
		}
	}
}
void PurpleBall::checkCollisionEntity()
{
	for (auto it : *entities)
	{
		if (it->getType() == enemy && getRect().intersects(it->getRect()))
		{
			if (curentFrame < 8.f)
			{
				direction = hitLeft;
				curentFrame = 8.f;
				it->setInjury(prop.getDamage(), shotAuthorID);
			}
			else if (curentFrame > 9.f && curentFrame < 13.9f) dPos = { 0.f, 0.f };
			else if (curentFrame > 13.9f) isLife = false;

			break;
		}
	}
}

class GainArmor : public Entity
{
private:
	std::list <Entity*>* entities;
	int armor,
		duration;

	void switchDirection();
	void setInjury(int, int){}
public:
	GainArmor(String = "", String = "", int = 0, std::vector<tmx::MapObject>* _collisionObj = nullptr, std::list <Entity*>* _entities = nullptr, int _shotAuthorID = 0, Vector2f _target = { 0.f, 0.f });

	void update(float _time);
};
GainArmor::GainArmor(String, String, int, std::vector<tmx::MapObject>* _collisionObj, std::list <Entity*>* _entities, int _shotAuthorID, Vector2f _target) :Entity("", "", _collisionObj)
{
	int arrGainArmor[20] = { 0, 5, 7, 10, 13, 16, 20, 24, 28, 32, 36, 40, 44, 48, 50, 51, 52, 53, 54, 55 };
	int arrDuration[20] = { 0, 5000, 5500, 6000, 6500, 6700, 7000, 7200, 7500, 7700, 8000, 8500, 9000, 9300, 9400, 9500, 9600, 9700, 9800, 10000 };
	int arrRegeneration[20] = { 0, 20000, 20000, 22000, 25000, 30000, 35000, 40000, 45000, 50000, 55000, 60000, 65000, 70000, 75000, 80000, 85000, 90000, 100000, 110000 };

	type = skillEntity;
	typeSkill = skillType::gainArmor;
	entities = _entities;
	shotAuthorID = _shotAuthorID;
	direction = left;
	width = 97.f;
	height = 97.f;
	prop.speed = 0.f;

	image.loadFromFile("images/skill/gainArmor.png");
	texture.loadFromImage(image);
	sprite.setTexture(texture);

	for (auto it : *entities)
	{
		if (it->getID() == shotAuthorID)
		{
			prop.level = it->getProperties().skills.getSkillLevel(typeSkill);
			armor = arrGainArmor[prop.level];
			for (int i = 0; i < 7; ++i)
			{
				if (it->getProperties().fast.fastSkill[i] == typeSkill)
				{
					it->getProperties().fast.fastLoad[i] = arrRegeneration[prop.level];
					break;
				}
			}
			it->getProperties().dress.chest.armor += armor;
			duration = arrDuration[prop.level];
			break;
		}
	}
}
void GainArmor::update(float _time)
{
	updateTime = _time;
	duration -= updateTime;

	for (auto it : *entities)
	{
		if (it->getID() == shotAuthorID)
		{
			position = Vector2f{ it->getRect().left + it->getRect().width / 2.f - width / 2.f, it->getRect().top + it->getRect().height / 2.f - height };
			break;
		}
	}
	sprite.setPosition(position);

	switchDirection();

	if (duration < 0)
	{
		isLife = false;
		for (auto it : *entities)
		{
			if (it->getID() == shotAuthorID)
			{
				it->getProperties().dress.chest.armor -= armor;
				break;
			}
		}
	}
}
void GainArmor::switchDirection()
{
	switch (direction)
	{
	case left:
	{
		curentFrame += 0.015f*updateTime;
		if ((int)curentFrame == 0){ sprite.setTextureRect(IntRect(1, 0, 97, 100)); }
		else if ((int)curentFrame == 1){ sprite.setTextureRect(IntRect(101, 0, 97, 100)); }
		else if ((int)curentFrame == 2){ sprite.setTextureRect(IntRect(200, 0, 97, 100)); }
		else if ((int)curentFrame == 3){ sprite.setTextureRect(IntRect(299, 0, 97, 100)); }
		else if ((int)curentFrame == 4){ sprite.setTextureRect(IntRect(398, 0, 97, 100)); }
		else if ((int)curentFrame == 5){ sprite.setTextureRect(IntRect(498, 0, 97, 100)); }
		else if ((int)curentFrame == 6){ sprite.setTextureRect(IntRect(597, 0, 97, 100)); }
		else if ((int)curentFrame == 7){ sprite.setTextureRect(IntRect(696, 0, 97, 100)); }
		else if ((int)curentFrame == 8){ sprite.setTextureRect(IntRect(796, 0, 97, 100)); }
		else if ((int)curentFrame == 9){ sprite.setTextureRect(IntRect(895, 0, 97, 100)); }
		else if ((int)curentFrame == 10){ sprite.setTextureRect(IntRect(994, 0, 97, 100)); }
		else if ((int)curentFrame == 11){ sprite.setTextureRect(IntRect(1094, 0, 97, 100)); }
		else if ((int)curentFrame == 12){ sprite.setTextureRect(IntRect(1193, 0, 97, 100)); }
		else if ((int)curentFrame == 13){ sprite.setTextureRect(IntRect(1292, 0, 97, 100)); }
		else if ((int)curentFrame == 14){ sprite.setTextureRect(IntRect(1391, 0, 97, 100)); }
		else if ((int)curentFrame == 15){ sprite.setTextureRect(IntRect(1491, 0, 97, 100)); }
		else if ((int)curentFrame == 16){ sprite.setTextureRect(IntRect(1590, 0, 97, 100)); }
		else if ((int)curentFrame == 17){ sprite.setTextureRect(IntRect(1689, 0, 97, 100)); }
		else if ((int)curentFrame == 18){ sprite.setTextureRect(IntRect(1788, 0, 97, 100)); }
		else { sprite.setTextureRect(IntRect(1788, 0, 10, 10)); }
		break;
	}
	default:
		break;
	}
}


class Player : public Entity
{
private:
	std::list <Entity*>* entities;
	Cursor* cursor;
	int useSkill;
	void loadProperties();
	void control();
	void switchDirection();
	void checkCollision(Vector2f);
	void checkCollisionMap(Vector2f);
	void checkCollisionEnemy(Vector2f);
	void setProperties();
	void circumventObstacle(FloatRect _obstacle);
public:
	Player();
	Player(String, String, std::list <Entity*>*, std::vector<tmx::MapObject>*, Vector2f, Cursor*);
	~Player();

	void update(float);
	void setInjury(int _injury, int _shotAuthorID);
	void switchSkill(int _fast, Vector2f _mousePosition = { 0.f, 0.f });
	void plEvent(Event& event, Vector2f _mousePos);
};
Player::Player() :Entity(){};
Player::Player(String _image, String name, std::list <Entity*>* _entities, std::vector<tmx::MapObject>* _collisionObj, Vector2f position, Cursor* _cursor) :Entity(_image, name, _collisionObj, position)
{
	type = player;
	direction = stay;
	entities = _entities;
	cursor = _cursor;
	width = 20.f;
	height = 32.f;
	useSkill = 0;
	loadProperties();	

	sprite.setTextureRect(IntRect(70, 65, (int)width, (int)height));
	sprite.setPosition(position.x + width / 2, position.y + height / 2);
}
void Player::loadProperties()
{
	prop.experience = 0;
	prop.level = 0;
	prop.experienceProgress = 0.f;
	prop.health = 100;

	prop.dress.rightHand.damageMin = 8;
	prop.dress.rightHand.damageMax = 12;
	prop.dress.rightHand.attackMin = 5;
	prop.dress.rightHand.attackMax = 7;
	prop.dress.rightHand.armor = 0;
	prop.dress.rightHand.criticalChance = 1;

	prop.dress.leftHand.damageMin = 0;
	prop.dress.leftHand.damageMax = 0;
	prop.dress.leftHand.attackMin = 0;
	prop.dress.leftHand.attackMax = 0;
	prop.dress.leftHand.armor = 0;
	prop.dress.leftHand.criticalChance = 0;

	prop.dress.chest.damageMin = 0;
	prop.dress.chest.damageMax = 0;
	prop.dress.chest.attackMin = 0;
	prop.dress.chest.attackMax = 0;
	prop.dress.chest.armor = 0;
	prop.dress.chest.criticalChance = 0;

	prop.skills.setSkillLevel(skillType::gainArmor, 1);
	prop.skills.setSkillLevel(skillType::fireBall, 1);
	prop.skills.setSkillLevel(skillType::fireBurst, 1);
	prop.skills.setSkillLevel(skillType::purpleBall, 1);
	prop.skills.setSkillLevel(skillType::horror, 0);
	prop.skills.setSkillLevel(skillType::lance, 0);
	prop.skills.setSkillLevel(skillType::silence, 0);
	prop.skills.setSkillLevel(skillType::strength, 0);
	prop.skills.setSkillLevel(skillType::sword, 1);
	prop.skills.setSkillLevel(skillType::webThrow, 0);

	prop.fast.fastSkill[0] = skillType::sword;
	prop.fast.fastSkill[1] = skillType::gainArmor;
	prop.fast.fastSkill[2] = skillType::purpleBall;
	prop.fast.fastSkill[3] = skillType::fireBurst;
	prop.fast.fastSkill[4] = skillType::strength;
	prop.fast.fastSkill[5] = skillType::webThrow;
	prop.fast.fastSkill[6] = skillType::fireBall;
	for (int i = 0; i < 7; i++)
	{
		prop.fast.fastLoading[i] = 0;
		prop.fast.fastLoad[i] = 0;
	}

	prop.charakter.strength = 1;
	prop.charakter.dexterity = 1;
	prop.charakter.vitality = 1;
	prop.charakter.energy = 1;
}
void Player::update(float _time)
{
	updateTime = _time;

	if (targetPosition.x || targetPosition.y)
	{
		if (persecutionID)
		{
			for (auto it : *entities)
				if (it->getID() == persecutionID)
					setTargetPosition(it->getPosition());
		}
		dPos = targetVector*prop.speed;
		if (targetVector.y<0.707107f && targetVector.y>-0.707107f)
		{
			if (targetVector.x > 0)direction = right;
			else direction = left;
		}
		else
		{
			if (targetVector.y > 0)direction = down;
			else direction = up;
		}

		if (targetPosition.x - position.x < 1.f && targetPosition.x - position.x > -1.f && targetPosition.y - position.y < 1.f && targetPosition.y - position.y > -1.f)
		{
			direction = stay;
			targetPosition = { 0, 0 };
		}
	}

	switchDirection();
	setProperties();
}
void Player::switchDirection()
{
	if (isHit)
	{
		if (direction == up)direction = hitUp;
		else if (direction == down)direction = hitDown;
		else if (direction == right)direction = hitRight;
		else if (direction == left)direction = hitLeft;
		prop.speed = 0.f;
		dPos = { 0.f, 0.f };
		if (curentFrame < 4.f)curentFrame = 4.f;
	}

	switch (direction)
	{
	case stay:
	{
		prop.speed = 0.f;
		curentFrame += 0.0015f*updateTime;
		if (curentFrame > 3.0f)curentFrame = 0.0f;
		sprite.setTextureRect(IntRect(280 + (int)curentFrame * 24, 20, 20, 33));
		break;
	}
	case right:
	{
		prop.speed = 0.07f;
		curentFrame += 0.008f*updateTime;
		if (curentFrame > 3)curentFrame = 0;
		sprite.setTextureRect(IntRect(70 + (int)curentFrame * 26, 65, 20, 33));
		if (sprite.getScale().x < 0) sprite.setScale(1, 1);
		break;
	}
	case left:
	{
		prop.speed = 0.07f;
		curentFrame += 0.008f*updateTime;
		if (curentFrame > 3)curentFrame = 0;
		sprite.setTextureRect(IntRect(70 + (int)curentFrame * 26, 65, 20, 33));
		if (sprite.getScale().x > 0) sprite.setScale(-1, 1);
		break;
	}
	case up:
	{
		prop.speed = 0.07f;
		curentFrame += 0.008f*updateTime;
		if (curentFrame > 3.0f)curentFrame = 0.0f;
		sprite.setTextureRect(IntRect(70 + (int)curentFrame * 23, 153, 19, 33));
		break;
	}
	case down:
	{
		prop.speed = 0.07f;
		curentFrame += 0.008f*updateTime;
		if (curentFrame > 3.0f)curentFrame = 0.0f;
		sprite.setTextureRect(IntRect(70 + (int)curentFrame * 22, 109, 19, 33));
		break;
	}
	case leftUp:
	{
		prop.speed = 0.07f;
		curentFrame += 0.008f*updateTime;
		if (curentFrame > 3.0f)curentFrame = 0.0f;
		sprite.setTextureRect(IntRect(70 + (int)curentFrame * 23, 153, 19, 33));
		break;
	}
	case rightUp:
	{
		prop.speed = 0.07f;
		curentFrame += 0.008f*updateTime;
		if (curentFrame > 3.0f)curentFrame = 0.0f;
		sprite.setTextureRect(IntRect(70 + (int)curentFrame * 23, 153, 19, 33));
		break;
	}
	case leftDown:
	{
		prop.speed = 0.07f;
		curentFrame += 0.008f*updateTime;
		if (curentFrame > 3.0f)curentFrame = 0.0f;
		sprite.setTextureRect(IntRect(70 + (int)curentFrame * 22, 109, 19, 33));
		break;
	}
	case rightDown:
	{
		prop.speed = 0.07f;
		curentFrame += 0.008f*updateTime;
		if (curentFrame > 3.0f)curentFrame = 0.0f;
		sprite.setTextureRect(IntRect(70 + (int)curentFrame * 22, 109, 19, 33));
		break;
	}
	case hitUp:
	{
		curentFrame += 0.005f*updateTime;
		if ((int)curentFrame == 4)sprite.setTextureRect(IntRect(32, 333, 20, 32));
		else if ((int)curentFrame == 5)sprite.setTextureRect(IntRect(59, 333, 20, 32));
		else if ((int)curentFrame == 6)sprite.setTextureRect(IntRect(87, 333, 42, 32));
		else if ((int)curentFrame == 7)sprite.setTextureRect(IntRect(134, 333, 33, 32));
		else if ((int)curentFrame == 8)sprite.setTextureRect(IntRect(175, 333, 29, 32));
		break;
	}
	case hitDown:
	{
		curentFrame += 0.005f*updateTime;
		if ((int)curentFrame == 4)sprite.setTextureRect(IntRect(20, 277, 22, 32));
		else if ((int)curentFrame == 5)sprite.setTextureRect(IntRect(52, 277, 20, 32));
		else if ((int)curentFrame == 6)sprite.setTextureRect(IntRect(82, 285, 37, 35));
		else if ((int)curentFrame == 7)sprite.setTextureRect(IntRect(126, 287, 32, 35));
		else if ((int)curentFrame == 8)sprite.setTextureRect(IntRect(166, 288, 28, 23));
		break;
	}
	case hitRight:
	{
		curentFrame += 0.005f*updateTime;
		if ((int)curentFrame == 4)sprite.setTextureRect(IntRect(27, 379, 16, 29));
		else if ((int)curentFrame == 5)sprite.setTextureRect(IntRect(58, 381, 21, 29));
		else if ((int)curentFrame == 6)sprite.setTextureRect(IntRect(86, 381, 36, 29));
		else if ((int)curentFrame == 7)sprite.setTextureRect(IntRect(128, 381, 37, 29));
		else if ((int)curentFrame == 8)sprite.setTextureRect(IntRect(171, 381, 37, 29));
		break;
	}
	case hitLeft:
	{
		curentFrame += 0.005f*updateTime;
		if ((int)curentFrame == 4){ sprite.setTextureRect(IntRect(27, 379, 16, 29)); sprite.setScale(-1.f, 0.f); }
		else if ((int)curentFrame == 5){ sprite.setTextureRect(IntRect(58, 381, 21, 29)); sprite.setScale(-1.f, 0.f); }
		else if ((int)curentFrame == 6){ sprite.setTextureRect(IntRect(86, 381, 36, 29)); sprite.setScale(-1.f, 0.f); }
		else if ((int)curentFrame == 7){ sprite.setTextureRect(IntRect(128, 381, 37, 29)); sprite.setScale(-1.f, 0.f); }
		else if ((int)curentFrame == 8){ sprite.setTextureRect(IntRect(171, 381, 37, 29)); sprite.setScale(-1.f, 0.f); }
		break;
	}
	default:
		break;
	}

	if (direction)
	{
		position.x += dPos.x*updateTime;
		checkCollision({ dPos.x, 0.0f });
		position.y += dPos.y*updateTime;
		checkCollision({ 0.0f, dPos.y });
		sprite.setPosition(position.x + width / 2.0f, position.y + height / 2.0f);
		if (prop.health < 1)isLife = false;
	}
}
void Player::checkCollision(Vector2f _dPos)
{
	checkCollisionMap(_dPos);
	checkCollisionEnemy(_dPos);
}
inline void Player::checkCollisionMap(Vector2f _dPos)
{
	for (auto it : *collisionObj)
	{
		if (getRect().intersects(it.GetAABB()))
		{
			//circumventObstacle(it->rect); //not work (:

			if (_dPos.y > 0.0f){ position.y = it.GetAABB().top - height; dPos.y = 0.0f; }
			else if (_dPos.y < 0.0f){ position.y = it.GetAABB().top + it.GetAABB().height; dPos.y = 0.0f; }
			else if (_dPos.x > 0.0f){ position.x = it.GetAABB().left - width; dPos.x = 0.0f; }
			else if (_dPos.x < 0.0f){ position.x = it.GetAABB().left + it.GetAABB().width; dPos.x = 0.0f; }

			if (it.GetAABB().contains(targetPosition))
			{
				targetPosition = { 0.f, 0.f };
				direction = stay;
			}
			else setTargetPosition(targetPosition);		
		}
	}
}
inline void Player::checkCollisionEnemy(Vector2f _dPos)
{
	for (auto it = entities->begin(); it != entities->end(); it++)
	{
		if (getRect().intersects((*it)->getRect()))
		{
			if ((*it)->getType() == enemy)
			{
				if ((*it)->getID() == persecutionID)
				{
					if (!isHit)
					{
						isHit = true;
						setTargetPosition({ 0.f, 0.f });

					}
					else
					{
						if (curentFrame > 8.5f)(*it)->setInjury(prop.getDamage(), id);
					}
				}
				else
				{
					if (!isHit)
					{
						if (_dPos.y > 0.0f){ /*position.y = (*it)->getRect().top - height;*/ dPos.y = 0.0f; }
						else if (_dPos.y < 0.0f){ /*position.y = (*it)->getRect().top + (*it)->getRect().height;*/ dPos.y = 0.0f; }
						else if (_dPos.x > 0.0f){ /*position.x = (*it)->getRect().left - width;*/ dPos.x = 0.0f; }
						else if (_dPos.x < 0.0f){ /*position.x = (*it)->getRect().left + (*it)->getRect().width;*/ dPos.x = 0.0f; }
					}
				}
			}
		}
	}
	if (curentFrame > 8.5f)
	{
		persecutionID = 0;
		isHit = false;
		curentFrame = 0.f;
		direction = stay;
	}
}
void Player::circumventObstacle(FloatRect _obstacle)
{
	if (_obstacle.contains({ position.x + width + 1, position.y + height }))		//препятствие снизу
	{
		if (position.x + width - _obstacle.left < _obstacle.left + _obstacle.width - position.x)dPos = { -prop.speed, 0.f };
		else dPos = { prop.speed, 0.f };
		position.y = _obstacle.top - height;
	}
	else if (_obstacle.contains({ position.x + width + 1, position.y }))				//препятствие сверху
	{
		if (position.x + width - _obstacle.left < _obstacle.left + _obstacle.width - position.x)dPos = { -prop.speed, 0.f };
		else dPos = { prop.speed, 0.f };
		position.y = _obstacle.top + _obstacle.height;
	}
	else if (_obstacle.contains({ position.x + width, position.y + height + 1 }))				//препятствие справа
	{
		if (position.y + height - _obstacle.top < _obstacle.top + _obstacle.height - position.y)dPos = { 0.f, -prop.speed };
		else dPos = { 0.f, prop.speed };
		position.x = _obstacle.left - width;
	}
	else if (_obstacle.contains({ position.x, position.y + height + 1 }))				//препятствие слева
	{
		if (position.y + height - _obstacle.top < _obstacle.top + _obstacle.height - position.y)dPos = { 0.f, -prop.speed };
		else dPos = { 0.f, prop.speed };
		position.x = _obstacle.left + _obstacle.width;
	}
}
void Player::setProperties()
{
	for (int i = 0; i < 7; ++i)
		if (prop.fast.fastLoad[i])
		{
			prop.fast.fastLoading[i] += updateTime;
			if (prop.fast.fastLoading[i] >= prop.fast.fastLoad[i])
			{
				prop.fast.fastLoading[i] = prop.fast.fastLoad[i] = 0;
			}
		}
}
void Player::setInjury(int _injury, int _shotAuthorID)
{
	float countInjury = _injury*(1 - prop.getArmor() / 100);
	char strDamage[10]; _itoa_s((int)countInjury, strDamage, 10);
	prop.health -= countInjury;
	isLife = (prop.health > 0) ? true : false;
	entities->push_back(new Lettering(strDamage, "lifeDown", { position.x, position.y - height / 2 }));
	Load_Singleton::getInstance()->getSoundPlayerDamage()->play();
}
void Player::plEvent(Event& event, Vector2f _mousePos)
{
	if (event.type == Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == Mouse::Right)
		{
			if (useSkill)
			{
				useSkill = 0;
				cursor->setCursor();
			}
			else if (prop.fast.fastLoad[6] == 0)
			{
				useSkill = 6;
				switchSkill(useSkill, _mousePos);
			}
		}

		else if (event.mouseButton.button == Mouse::Left)
		{
			if (useSkill)
			{
				switchSkill(useSkill, _mousePos);
			}
			else
			{
				for (auto it = entities->begin(); it != entities->end(); ++it)
				{
					if ((*it)->getType() == enemy)
						if ((*it)->getRect().contains(_mousePos))
							setPersecution((*it)->getID());
						else setPersecution(0);
				}
				setTargetPosition(_mousePos);
			}
		}
	}
	else if (event.type == Event::KeyPressed)
	{
		switchSkill(event.key.code - 26, _mousePos);
	}
}
void Player::switchSkill(int _fast, Vector2f _mousePos)
{
	if (!prop.fast.fastLoad[_fast])
		switch (prop.fast.fastSkill[_fast])
	{
		case skillType::fireBall:
		{
			if (useSkill == 0)
			{
				useSkill = _fast;
				cursor->setCursor(Cursor::cursorType::skillCur);
			}
			else
			{
				entities->push_back(new FireBall("", "", 0, collisionObj, entities, id, _mousePos));
				useSkill = 0;
				cursor->setCursor();
			}
			break;
		}
		case skillType::fireBurst:
			if (useSkill == 0)
			{
				useSkill = _fast;
				cursor->setCursor(Cursor::cursorType::skillCur);
			}
			else
			{
				entities->push_back(new FireBurst("", "", 0, collisionObj, entities, id, _mousePos));
				useSkill = 0;
				cursor->setCursor();
			}
			break;
		case skillType::purpleBall:
			if (useSkill == 0)
			{
				useSkill = _fast;
				cursor->setCursor(Cursor::cursorType::skillCur);
			}
			else
			{
				entities->push_back(new PurpleBall("", "", 0, collisionObj, entities, id, _mousePos));
				useSkill = 0;
				cursor->setCursor();
			}
			break;
		case skillType::gainArmor:
			entities->push_back(new GainArmor("", "", 0, collisionObj, entities, id));
			break;

		default:
			break;
	}
}

class Enemy : public Entity
{
private:
	bool isBarrier,
		isPlayerBeside;
	std::list <Entity*>* entities;
	void enemy1Update();
	void checkCollision(Vector2f);
	void checkCollisionMap();
	bool isObstacle(Vector2f, FloatRect);
	bool isLinesCross(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22);
public:
	Enemy(String, String, std::list <Entity*>*, std::vector<tmx::MapObject>*, Vector2f);
	~Enemy();

	void update(float);
	void setInjury(int _injury, int _shotAuthorID);
};
Enemy::Enemy(String _image, String name, std::list <Entity*>* _entities, std::vector<tmx::MapObject>* _collisionObj, Vector2f position) :Entity(_image, name, _collisionObj, position)
{
	type = enemy;
	isPlayerBeside = false;
	isBarrier = false;
	entities = _entities;
	if (name == "EasyEnemy")
	{
		width = 32.0f;
		height = 40.0f;
		sprite.setTextureRect(IntRect(0, 0, (int)width, (int)height));
		dPos = { 0.1f, 0.0f };

		{
			prop.speed = 0.06f;
			prop.visionRange = 100.0f;
			prop.experience = 1;
			prop.health = 30;

			prop.dress.rightHand.damageMin = 5;
			prop.dress.rightHand.damageMax = 7;
			prop.dress.chest.armor = 5;
		}

	}
}
Enemy::~Enemy(){}
void Enemy::update(float _time)
{
	updateTime = _time;
	if (name == "EasyEnemy")
	{
		enemy1Update();

	}
}
void Enemy::enemy1Update()
{
	for (auto it = entities->begin(); it != entities->end(); ++it)
	{
		if ((*it)->getType() == player)
		{
			targetDistance = sqrt(((*it)->getPosition().x - position.x)*((*it)->getPosition().x - position.x) + ((*it)->getPosition().y - position.y)*((*it)->getPosition().y - position.y));
			if (targetDistance < prop.visionRange)
			{
				isPlayerBeside = true;
				targetPosition = ((*it)->getPosition() - position) / targetDistance*prop.speed;
				isBarrier = false;
				for (auto it2 : *collisionObj)
				{
					isBarrier = isObstacle({ (*it)->getRect().left + (*it)->getRect().width / 2, (*it)->getRect().top + (*it)->getRect().height / 2 }, it2.GetAABB());
					if (isBarrier)break;
				}
			}
			else isPlayerBeside = false;
		}
	}
	Vector2f tempDPos = dPos;
	if (isPlayerBeside && !isBarrier){ dPos = targetPosition; }
	else
	{
		if (tempDPos.x > 0) dPos = { prop.speed, 0 };
		else dPos = { -prop.speed, 0 };
	}

	curentFrame += 0.0015f*updateTime;
	if (!isHit)
		if (curentFrame > 3.5f)curentFrame = 0.0f;
	if ((int)curentFrame == 0)
	{
		checkCollision(dPos);
		position += dPos*updateTime;
		sprite.setTextureRect(IntRect(0, 0, (int)width, (int)height));
	}
	else if ((int)curentFrame == 1)
	{
		checkCollision(dPos);
		position += dPos*updateTime;
		sprite.setTextureRect(IntRect(32, 0, (int)width, (int)height));
	}
	else if ((int)curentFrame == 2)
	{
		checkCollision(dPos);
		position += dPos*updateTime;
		sprite.setTextureRect(IntRect(64, 0, (int)width, (int)height));
	}
	else if ((int)curentFrame == 3)
	{
		checkCollision(dPos);
		position += dPos*updateTime;
		sprite.setTextureRect(IntRect(96, 0, (int)width, (int)height));
	}
	else if ((int)curentFrame == 4){ checkCollision(dPos); sprite.setTextureRect(IntRect(161, 0, 14, 14)); }
	else if ((int)curentFrame == 5){ checkCollision(dPos); sprite.setTextureRect(IntRect(176, 0, 37, 37)); }
	else if ((int)curentFrame == 6){ checkCollision(dPos); sprite.setTextureRect(IntRect(217, 0, 54, 54)); }
	else if ((int)curentFrame == 7)
	{
		checkCollision(dPos);
		isHit = false;
	}
	sprite.setPosition(position.x + width / 2.0f, position.y + height / 2.0f);
}
void Enemy::checkCollision(Vector2f _dPos)
{
	checkCollisionMap();

	for (auto it = entities->begin(); it != entities->end(); it++)
	{
		if (getRect().intersects((*it)->getRect()))
		{
			if ((*it)->getType() == player)
			{
				if (!isHit)
				{
					curentFrame = 4;
					isHit = true;
				}
				else if (curentFrame > 7.f) (*it)->setInjury(prop.getDamage(), id);
			}
		}
	}
}
void Enemy::setInjury(int _injury, int _shotAuthorID)
{
	float countInjury = _injury*(1 - prop.getArmor() / 100);
	char strDamage[10]; _itoa_s((int)countInjury, strDamage, 10);
	prop.health -= countInjury;

	if (prop.health <= 0)
	{
		isLife = false;
		for (auto it = entities->begin(); it != entities->end(); it++)
		{
			if (_shotAuthorID == (*it)->getID())
			{
				(*it)->getProperties().setUpExperience(prop.experience);
				break;
			}
		}
	}

	entities->push_back(new Lettering(strDamage, "lifeDown", { position.x, position.y - height / 2 }));
	Load_Singleton::getInstance()->getSoundPlayerDamage()->play();
}
inline void Enemy::checkCollisionMap()
{
	for (auto it : *collisionObj)
	{
		if (getRect().intersects(it.GetAABB()))
		{
			if (getRect().top + getRect().height - it.GetAABB().top < 1.0f)
			{
				//вставить код, если enemy сам двигаетс¤ по y
				if (isPlayerBeside && !isBarrier)dPos.y = 0;
			}
			else if (getRect().top - (it.GetAABB().top + it.GetAABB().height) < 1.0f)
			{
				//вставить код, если enemy сам двигаетс¤ по y
				if (isPlayerBeside && !isBarrier)dPos.y = 0;
			}
			if (getRect().left + getRect().width - it.GetAABB().left < 1.5f && getRect().left + getRect().width - it.GetAABB().left > -1.5f)
			{
				if (!(isPlayerBeside && !isBarrier)){ position.x = it.GetAABB().left - width; dPos.x = -0.1f; sprite.scale(-1.0f, 1.0f); }
				else { position.x = it.GetAABB().left - width; dPos.x = 0; }
			}
			else if (getRect().left - (it.GetAABB().left + it.GetAABB().width) < 1.5f && getRect().left - (it.GetAABB().left + it.GetAABB().width) > -1.5f)
			{
				if (!(isPlayerBeside && !isBarrier)){ position.x = it.GetAABB().left + it.GetAABB().width; dPos.x = 0.1f; sprite.scale(-1.0f, 1.0f); }
				else { position.x = it.GetAABB().left + it.GetAABB().width; dPos.x = 0; }
			}
		}
	}
}
inline bool Enemy::isObstacle(Vector2f plPos, FloatRect _rect)
{
	bool _isIntersect = false;

	for (int i = 0; i < 4; i++)
	{
		Vector2f c, d;
		if (i < 2) c = { _rect.left, _rect.top }; else c = { _rect.left + _rect.width, _rect.top + _rect.height };
		if (i % 2) d = { _rect.left + _rect.width, _rect.top }; else d = { _rect.left, _rect.top + _rect.height };
		_isIntersect = isLinesCross(plPos.x, plPos.y, position.x + width / 2, position.y + height / 2, c.x, c.y, d.x, d.y);
		if (_isIntersect)break;
	}

	return _isIntersect;
}
inline bool Enemy::isLinesCross(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22)
{

	float maxx1 = std::max(x11, x12), maxy1 = std::max(y11, y12);
	float minx1 = std::min(x11, x12), miny1 = std::min(y11, y12);
	float maxx2 = std::max(x21, x22), maxy2 = std::max(y21, y22);
	float minx2 = std::min(x21, x22), miny2 = std::min(y21, y22);

	if (minx1 > maxx2 || maxx1 < minx2 || miny1 > maxy2 || maxy1 < miny2)
		return false;


	float dx1 = x12 - x11, dy1 = y12 - y11;
	float dx2 = x22 - x21, dy2 = y22 - y21;
	float dxx = x11 - x21, dyy = y11 - y21;
	float div, mul;


	if ((div = (int)(dy2*dx1 - dx2*dy1)) == 0)
		return false; // Ћинии параллельны...
	if (div > 0) {
		if ((mul = (dx1*dyy - dy1*dxx)) < 0 || mul > div)
			return false; // ѕервый отрезок пересекаетс¤ за своими границами...
		if ((mul = (dx2*dyy - dy2*dxx)) < 0 || mul > div)
			return false; // ¬торой отрезок пересекаетс¤ за своими границами...
	}

	if ((mul = -(dx1*dyy - dy1*dxx)) < 0 || mul > -div)
		return false; // ѕервый отрезок пересекаетс¤ за своими границами...
	if ((mul = -(dx2*dyy - dy2*dxx)) < 0 || mul > -div)
		return false; // ¬торой отрезок пересекаетс¤ за своими границами...

	return true;
}

class SF_Desk
{
public:
	enum SFGUI_Type{ mainFooter, skillsFooter, headMenu, optionMenu, skillTree };
private:
	Vector2f requesition;
	sfg::Desktop SF_Desktop;
	std::list <Entity*>* entities;
	Player* player;
	GameView* view;
	std::list<std::shared_ptr<SFGUI_Entity>>* sf_entities;

	void switchSFGUI_Entity(SFGUI_Type);

public:
	SF_Desk(std::list<std::shared_ptr<SFGUI_Entity>>* = nullptr, std::list <Entity*>* = nullptr, Player* = nullptr, GameView* = nullptr, Vector2f = { 0.f, 0.f });
	void create(std::list<std::shared_ptr<SFGUI_Entity>>* = nullptr, std::list <Entity*>* = nullptr, Player* = nullptr, GameView* = nullptr, Vector2f = { 0.f, 0.f });
	~SF_Desk();

	void showSFGUI(SFGUI_Type);
	Vector2f getWindowSize(){ return requesition; }
	sfg::Desktop& getDesk(){ return SF_Desktop; }
	void update();
};

class SFGUI_Entity
{
protected:
	SF_Desk::SFGUI_Type type;
	Vector2f position;
	Player* player;
	SF_Desk* sf_desktop;
	GameView* view;
	bool isNeedUpdate;
	sfg::Window::Ptr SF_Window;
	std::shared_ptr<sfg::Box> SF_Box;

public:
	enum STATE{ HIDE, SHOW, INACTIVE };
	SFGUI_Entity();

	SF_Desk::SFGUI_Type getType(){ return type; }
	sfg::Window::Ptr getWindow(){ return SF_Window; }
	FloatRect getRect(){ return SF_Window->GetAllocation(); }
	STATE getState(){ return state; }
	void setUpdate(bool _isNeedUpdate){ isNeedUpdate = _isNeedUpdate; }

	virtual void setState(STATE);
	virtual void setProgress(float _progress){}
	virtual void OnButtonClick(int _buttonNum){}
	virtual void footerSetSkill(skillType _type){}
	virtual void update() = 0;

protected:
	STATE state;
};
SFGUI_Entity::SFGUI_Entity()
{
	position = Vector2f{ 0.f, 0.f };
	player = nullptr;
	sf_desktop = nullptr;
	view = nullptr;
	state = SHOW;
}
void SFGUI_Entity::setState(STATE _state)
{
	state = _state;
	if (state == HIDE)
	{
		SF_Window->Show(false);
	}
	else if (state == SHOW)
	{
		SF_Window->Show(true);
		SF_Window->SetState(sfg::Widget::State::NORMAL);
	}
	else if (state == INACTIVE)
	{
		SF_Window->SetState(sfg::Widget::State::INSENSITIVE);
	}
}

class SFGUI_Picture :public Entity
{
private:
	Image image;
	SFGUI_Entity* sf_entity;
public:
	SFGUI_Picture(String = "", String = "", SFGUI_Entity* = nullptr, GameView* = nullptr);

	void update(float);
	void setInjury(int, int){}
};
SFGUI_Picture::SFGUI_Picture(String _image, String _name, SFGUI_Entity* _sf_entity, GameView* _view) :Entity(_image, _name, nullptr)
{
	type = picture;
	sf_entity = _sf_entity;
	view = _view;
	image.loadFromFile(_image);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	position = sf_entity->getWindow()->GetAbsolutePosition();
	position += view->getView().getCenter() - view->getView().getSize() / 2.f;
	sprite.setPosition(position);
	//std::cout << "x=" << sf_entity->getWindow()->GetAbsolutePosition().x << "; y=" << sf_entity->getWindow()->GetAbsolutePosition().y << std::endl;
}
void SFGUI_Picture::update(float)
{
	position = sf_entity->getWindow()->GetAbsolutePosition();
	position += view->getView().getCenter() - view->getView().getSize() / 2.f;
	sprite.setPosition(position);
}

class SF_ProgressBar
{
private:
	sfg::ProgressBar::Ptr m_progressbar;
	float progress;
	Vector2f requisition;
public:
	SF_ProgressBar(Vector2f _requisition = { 0.f, 0.f }) { create(_requisition); }
	void create(Vector2f _requisition)
	{
		progress = 0.f;
		m_progressbar = sfg::ProgressBar::Create();
		m_progressbar->SetRequisition(_requisition);
	}
	sfg::ProgressBar::Ptr getProgressBar(){ return m_progressbar; }
	void update(float _progress)
	{
		if (progress != _progress)
		{
			progress = _progress;
			m_progressbar->SetFraction(progress);
			m_progressbar->Update(0.f);
		}
	}
};

class SF_Button
{
private:
	String label,
		imageName;
	skillType type;
	bool isFastButton30,
		isLoad;
	Vector2f requisition;
	Image image;
	std::shared_ptr<sfg::Image> SF_Image;
	std::shared_ptr<sfg::Button> button;

	void setImageBlackWite(float _fraction);
	void switchSkill();
public:
	SF_Button(skillType _type = skillType::otherType, bool _isFastButton30 = false, String _label = "", Vector2f _requisition = { 0.f, 0.f })
	{
		create(_type, _isFastButton30, _label, _requisition);
	}
	void create(skillType _type, bool _isFastButton30 = false, String = "", Vector2f _requisition = { 0.f, 0.f });

	void steLabel(String _label = ""){ label = _label; button->SetLabel(label); }
	String getLabel(){ return label; }
	skillType getSkill(){ return type; }
	void setSkill(skillType _type);

	std::shared_ptr<sfg::Button> getButton(){ return button; }
	void update(int _load, int _loading);
};
void SF_Button::create(skillType _type, bool _isFastButton30, String _label, Vector2f _requisition)
{
	label = _label;
	type = _type;
	isFastButton30 = _isFastButton30;
	requisition = _requisition;
	isLoad = false;
	button = sfg::Button::Create(label);
	switchSkill();
	if (imageName != "")
	{
		imageName = (isFastButton30) ? imageName + "30.png" : imageName + "46.png";
		image.loadFromFile(imageName);
		SF_Image = sfg::Image::Create(image);
		button->SetImage(SF_Image);
	}
	button->SetRequisition(requisition);
}
void SF_Button::switchSkill()
{
	switch (type)
	{
	case skillType::fireBall:
		imageName = "images/skill/fireBall";
		break;
	case skillType::fireBurst:
		imageName = "images/skill/fireBurst";
		break;
	case skillType::purpleBall:
		imageName = "images/skill/purpleBall";
		break;
	case skillType::webThrow:
		imageName = "images/skill/webThrow";
		break;
	case skillType::strength:
		imageName = "images/skill/strength";
		break;
	case skillType::silence:
		imageName = "images/skill/silence";
		break;
	case skillType::horror:
		imageName = "images/skill/horror";
		break;
	case skillType::gainArmor:
		imageName = "images/skill/gainArmor";
		break;
	case skillType::lance:
		imageName = "images/skill/lance";
		break;
	case skillType::sword:
		imageName = "images/skill/sword";
		break;
	case skillType::noImage:
		imageName = "";
		break;
	case skillType::musicOn:
		imageName = "images/musicOn";
		break;
	case skillType::musicOff:
		imageName = "images/musicOff";
		break;
	default:
		imageName = "images/skill/emptySkill";
		break;
	}
}
void SF_Button::setImageBlackWite(float _fraction)
{
	image.loadFromFile(imageName);
	Vector2u size = image.getSize();
	for (int i = 0; i < size.x; i++)
		for (int j = 0; j < (size.y * _fraction); j++)
		{
			Color color = image.getPixel(i, j);
			color.r = color.g = color.b = (color.r + color.g + color.b) / 3;
			image.setPixel(i, j, color);
		}

}
void SF_Button::update(int _load, int _loading)
{
	if (_load)
	{
		isLoad = true;
		setImageBlackWite(1.f - (float)_loading / (float)_load);
		button->SetState(sfg::Widget::State::INSENSITIVE);

		SF_Image = sfg::Image::Create(image);
		button->SetImage(SF_Image);
		button->Update(0.f);
	}
	else
	{
		if (isLoad == true)
		{
			isLoad = false;
			button->SetState(sfg::Widget::State::NORMAL);

			image.loadFromFile(imageName);
			SF_Image = sfg::Image::Create(image);
			button->SetImage(SF_Image);
			button->Update(0.f);
		}
	}
}
void SF_Button::setSkill(skillType _type)
{
	type = _type;
	switchSkill();
	if (imageName != "")
	{
		imageName = (isFastButton30) ? imageName + "30.png" : imageName + "46.png";
		image.loadFromFile(imageName);
		SF_Image = sfg::Image::Create(image);
		button->SetImage(SF_Image);
	}
	button->Update(0.f);
}

class Footer :public SFGUI_Entity
{
private:
	SF_Button fastButton[7];
	SF_ProgressBar progressBarExperience;
	Image image;
	std::shared_ptr<sfg::Image> SFGUI_Image;
	Player* player;
	int fastRightClick;
public:
	Footer(Player* = nullptr, Vector2f = { 0.f, 0.f }, SF_Desk* = nullptr);
	void OnButtonClick(int _fast);
	void OnRightButtonClick(int _fast);
	void setProgress(float _experienceProgress);
	void footerSetSkill(skillType);
	void update();
};
Footer::Footer(Player* _player, Vector2f _position, SF_Desk* _sf_desktop) :SFGUI_Entity()
{
	type = SF_Desk::SFGUI_Type::mainFooter;
	player = _player;
	sf_desktop = _sf_desktop;
	SF_Window = sfg::Window::Create();
	SF_Window->SetStyle(sfg::Window::NO_STYLE);
	SF_Window->SetPosition(_position);
	SFGUI_Image = sfg::Image::Create();
	SF_Box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 3.f);
	progressBarExperience.create(Vector2f{ 200.f, 20.f });

	for (int i = 0; i < 7; i++)
	{
		if (i == 0 || i == 6)
			fastButton[i].create(player->getProperties().fast.fastSkill[i]);
		else
			fastButton[i].create(player->getProperties().fast.fastSkill[i], true);
	}

	auto alignment = sfg::Alignment::Create();
	auto alignment_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 1.f);
	auto boxFastButton = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 1.f);

	boxFastButton->Pack(fastButton[1].getButton());
	boxFastButton->Pack(fastButton[2].getButton());
	boxFastButton->Pack(fastButton[3].getButton());
	boxFastButton->Pack(fastButton[4].getButton());
	boxFastButton->Pack(fastButton[5].getButton());

	alignment_box->Pack(progressBarExperience.getProgressBar(), false);
	alignment_box->Pack(alignment, true, true);
	alignment->Add(boxFastButton);
	SF_Box->Pack(fastButton[0].getButton());
	SF_Box->Pack(alignment_box);
	SF_Box->Pack(fastButton[6].getButton());

	SF_Window->Add(SF_Box);
	for (int i = 0; i < 7; i++)
	{
		fastButton[i].getButton()->GetSignal(sfg::Widget::OnRightClick).Connect(std::bind(&Footer::OnRightButtonClick, this, i));
		fastButton[i].getButton()->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&Footer::OnButtonClick, this, i));
	}
}
void Footer::OnButtonClick(int _fast)
{
	player->switchSkill(_fast);
}
void Footer::OnRightButtonClick(int _fast)
{
	fastRightClick = _fast;
	sf_desktop->showSFGUI(SF_Desk::SFGUI_Type::skillsFooter);
}
void Footer::update()
{
	if (state == SHOW)
	{
		for (int i = 0; i < 7; i++)
		{
			fastButton[i].update(player->getProperties().fast.fastLoad[i], player->getProperties().fast.fastLoading[i]);
		}
		progressBarExperience.update(player->getProperties().experienceProgress);

		SF_Window->Update(0.f);
	}
}
void Footer::setProgress(float _experienceProgress)
{
	progressBarExperience.getProgressBar()->SetFraction(_experienceProgress);
}
void Footer::footerSetSkill(skillType _type)
{
	for (int i = 0; i < 7; ++i)
	{
		if (player->getProperties().fast.fastSkill[i] == _type)
		{
			player->getProperties().fast.fastSkill[i] = skillType::otherType;
			fastButton[i].setSkill(skillType::otherType);
		}
	}
	player->getProperties().fast.fastSkill[fastRightClick] = _type;
	fastButton[fastRightClick].setSkill(_type);
}

class SkillFooter :public SFGUI_Entity
{
private:
	SF_Button skillButton[20];
	int countSkillButtons;
	Image image;
	std::shared_ptr<sfg::Image> SFGUI_Image;
	std::list<std::shared_ptr<SFGUI_Entity>>* sf_entities;
	bool isNeedUpdate;
public:
	SkillFooter(Player* = nullptr, Vector2f = { 0.f, 0.f }, SF_Desk* = nullptr, std::list<std::shared_ptr<SFGUI_Entity>>* = nullptr);
	void OnButtonClick(int _buttonNum);
	void update();
};
SkillFooter::SkillFooter(Player* _player, Vector2f _position, SF_Desk* _sf_desktop, std::list<std::shared_ptr<SFGUI_Entity>>* _sf_entities)
{
	type = SF_Desk::SFGUI_Type::skillsFooter;
	isNeedUpdate = false;
	player = _player;
	sf_desktop = _sf_desktop;
	sf_entities = _sf_entities;
	SF_Window = sfg::Window::Create();
	SF_Window->SetStyle(sfg::Window::NO_STYLE);
	SF_Window->SetPosition(_position);
	SFGUI_Image = sfg::Image::Create();
	SF_Box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 3.f);

	countSkillButtons = 0;
	for (auto it = player->getProperties().skills.skillLevel.begin(); it != player->getProperties().skills.skillLevel.end(); ++it)
	{
		skillButton[countSkillButtons].create((*it)->type, true);
		if ((*it)->level == 0)
		{
			skillButton[countSkillButtons].getButton()->Show(false);
		}
		SF_Box->Pack(skillButton[countSkillButtons].getButton());
		countSkillButtons++;
	}
	countSkillButtons--;
	SF_Window->Add(SF_Box);

	for (int i = 0; i <= countSkillButtons; i++)
	{
		skillButton[i].getButton()->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&SkillFooter::OnButtonClick, this, i));
	}
}
void SkillFooter::OnButtonClick(int _buttonNum)
{
	for (auto it = sf_entities->begin(); it != sf_entities->end(); ++it)
	{
		if ((*it)->getType() == SF_Desk::SFGUI_Type::mainFooter)
		{
			(*it)->footerSetSkill(skillButton[_buttonNum].getSkill());
			break;
		}
	}
	sf_desktop->showSFGUI(SF_Desk::SFGUI_Type::mainFooter);
}
void SkillFooter::update()
{
	if (state == SHOW && isNeedUpdate)
	{
		isNeedUpdate = false;
		for (size_t i = 0; i <= countSkillButtons; i++)
		{
			for (auto it = player->getProperties().skills.skillLevel.begin(); it != player->getProperties().skills.skillLevel.end(); ++it)
			{
				if ((*it)->type == skillButton[i].getSkill())
				{
					if ((*it)->level == 0)skillButton[i].getButton()->Show(false);
					else skillButton[i].getButton()->Show(true);
					break;
				}
			}
		}
		SF_Window->Update(0.f);
	}
}

class OptionMenu : public SFGUI_Entity
{
private:
	std::shared_ptr<sfg::Adjustment> adjustmentMusic;
	std::shared_ptr<sfg::Adjustment> adjustmentSound;
	std::shared_ptr<sfg::Scale> scaleMusic;
	std::shared_ptr<sfg::Scale> scaleSound;
	std::shared_ptr<sfg::Label> labelMusic;
	std::shared_ptr<sfg::Label> labelSound;
	std::shared_ptr<sfg::Box> boxMusic;
	std::shared_ptr<sfg::Box> boxSound;
	std::shared_ptr<sfg::Box> boxMusic2;
	std::shared_ptr<sfg::Box> boxSound2;
	std::shared_ptr<sfg::Box> boxOkCancel;
	SF_Button buttonMusic;
	SF_Button buttonSound;
	SF_Button buttonSave;
	SF_Button buttonCancel;
	int volumeMusic,
		volumeSound;
	bool isButtonMusicOn,
		isButtonSoundOn;

public:
	OptionMenu(SF_Desk* = nullptr, Vector2f = { 0.f, 0.f });
	void adjustmentMusicChange();
	void adjustmentSoundChange();
	void OnButtonClick(int _buttonNum);
	void update(){
		if (state == SHOW && isNeedUpdate) { SF_Window->Update(0.f); isNeedUpdate = false; }
	}
};
OptionMenu::OptionMenu(SF_Desk* _sf_desktop, Vector2f _position) : SFGUI_Entity()
{
	type = SF_Desk::SFGUI_Type::optionMenu;
	sf_desktop = _sf_desktop;
	volumeMusic = volumeSound = 100;
	isButtonMusicOn = isButtonSoundOn = true;

	scaleMusic = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
	scaleSound = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
	scaleMusic->SetRequisition(Vector2f(80.f, 20.f));
	scaleSound->SetRequisition(Vector2f(80.f, 20.f));

	labelMusic = sfg::Label::Create();
	std::stringstream sstr;
	sstr << Load_Singleton::getInstance()->getMusic()->getVolume();
	labelMusic->SetText(sstr.str());

	labelSound = sfg::Label::Create();
	std::stringstream sstr2;
	sstr2 << Load_Singleton::getInstance()->getSoundVolume();
	labelSound->SetText(sstr2.str());

	adjustmentMusic = scaleMusic->GetAdjustment();
	adjustmentSound = scaleSound->GetAdjustment();
	adjustmentMusic->SetLower(0.f);
	adjustmentMusic->SetUpper(100.f);
	adjustmentMusic->SetValue(Load_Singleton::getInstance()->getMusic()->getVolume());
	adjustmentSound->SetLower(0.f);
	adjustmentSound->SetUpper(100.f);
	adjustmentSound->SetValue(Load_Singleton::getInstance()->getSoundVolume());
	adjustmentMusic->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&OptionMenu::adjustmentMusicChange, this));
	adjustmentSound->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&OptionMenu::adjustmentSoundChange, this));

	buttonMusic.create(skillType::musicOn, true);
	buttonSound.create(skillType::musicOn, true);
	buttonSave.create(skillType::noImage, false, "Save");
	buttonCancel.create(skillType::noImage, false, "Cancel");
	buttonMusic.getButton()->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&OptionMenu::OnButtonClick, this, 0));
	buttonSound.getButton()->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&OptionMenu::OnButtonClick, this, 1));
	buttonSave.getButton()->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&OptionMenu::OnButtonClick, this, 2));
	buttonCancel.getButton()->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&OptionMenu::OnButtonClick, this, 3));

	boxMusic = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 3.f);
	boxSound = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 3.f);
	boxMusic2 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 3.f);
	boxSound2 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 3.f);
	boxOkCancel = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 30.f);

	boxMusic->Pack(buttonMusic.getButton());
	boxMusic2->Pack(labelMusic);
	boxMusic2->Pack(scaleMusic);
	boxMusic->Pack(boxMusic2);

	boxSound->Pack(buttonSound.getButton());
	boxSound2->Pack(labelSound);
	boxSound2->Pack(scaleSound);
	boxSound->Pack(boxSound2);

	boxOkCancel->Pack(buttonCancel.getButton());
	boxOkCancel->Pack(buttonSave.getButton());

	SF_Window = sfg::Window::Create();
	SF_Window->SetStyle(sfg::Window::NO_STYLE);
	SF_Window->SetPosition(position);
	SF_Box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 3.f);

	SF_Box->Pack(boxMusic);
	SF_Box->Pack(boxSound);
	SF_Box->Pack(boxOkCancel);
	SF_Window->Add(SF_Box);
}
void OptionMenu::adjustmentMusicChange()
{
	std::stringstream sstrMusic;
	sstrMusic << adjustmentMusic->GetValue();
	labelMusic->SetText(sstrMusic.str());
	Load_Singleton::getInstance()->getMusic()->setVolume(adjustmentMusic->GetValue());
}
void OptionMenu::adjustmentSoundChange()
{
	std::stringstream sstrSound;
	sstrSound << adjustmentSound->GetValue();
	labelSound->SetText(sstrSound.str());
	Load_Singleton::getInstance()->setSoundVolume(adjustmentSound->GetValue());
}
void OptionMenu::OnButtonClick(int _buttonNum)
{
	switch (_buttonNum)
	{
	case 0:
		if (buttonMusic.getSkill() == skillType::musicOff)
		{
			buttonMusic.setSkill(skillType::musicOn);
			scaleMusic->SetState(sfg::Widget::State::NORMAL);
			Load_Singleton::getInstance()->getMusic()->setVolume(adjustmentMusic->GetValue());
		}
		else
		{
			buttonMusic.setSkill(skillType::musicOff);
			scaleMusic->SetState(sfg::Widget::State::INSENSITIVE);
			Load_Singleton::getInstance()->getMusic()->setVolume(0.f);
		}
		break;
	case 1:
		if (buttonSound.getSkill() == skillType::musicOff)
		{
			buttonSound.setSkill(skillType::musicOn);
			scaleSound->SetState(sfg::Widget::State::NORMAL);
			Load_Singleton::getInstance()->setSoundVolume(adjustmentSound->GetValue());
		}
		else
		{
			buttonSound.setSkill(skillType::musicOff);
			scaleSound->SetState(sfg::Widget::State::INSENSITIVE);
			Load_Singleton::getInstance()->setSoundVolume(0.f);
		}
		break;
	case 2:
		volumeMusic = adjustmentMusic->GetValue();
		volumeSound = adjustmentSound->GetValue();
		isButtonMusicOn = (buttonMusic.getSkill() == skillType::musicOn) ? true : false;
		isButtonSoundOn = (buttonSound.getSkill() == skillType::musicOn) ? true : false;
		sf_desktop->showSFGUI(SF_Desk::SFGUI_Type::headMenu);
		break;
	case 3:
	{
		adjustmentMusic->SetValue((float)volumeMusic);
		adjustmentSound->SetValue((float)volumeSound);

		buttonMusic.setSkill((isButtonMusicOn) ? skillType::musicOn : skillType::musicOff);
		scaleMusic->SetState((isButtonMusicOn) ? sfg::Widget::State::NORMAL : sfg::Widget::State::INSENSITIVE);
		buttonSound.setSkill((isButtonSoundOn) ? skillType::musicOn : skillType::musicOff);
		scaleSound->SetState((isButtonSoundOn) ? sfg::Widget::State::NORMAL : sfg::Widget::State::INSENSITIVE);

		Load_Singleton::getInstance()->getMusic()->setVolume((isButtonMusicOn) ? (float)volumeMusic : 0.f);
		Load_Singleton::getInstance()->setSoundVolume((isButtonSoundOn) ? (float)volumeSound : 0.f);

		std::stringstream sstrMusic;
		sstrMusic << adjustmentMusic->GetValue();
		labelMusic->SetText(sstrMusic.str());
		std::stringstream sstrSound;
		sstrSound << adjustmentSound->GetValue();
		labelSound->SetText(sstrSound.str());
		sf_desktop->showSFGUI(SF_Desk::SFGUI_Type::headMenu);
		break;
	}
	default:
		break;
	}
}

class MainMenu :public SFGUI_Entity
{
private:
	SF_Button menuButton[6];
	std::list <Entity*>* entities;
public:
	MainMenu(Vector2f = { 0.f, 0.f }, std::list <Entity*>* = nullptr, SF_Desk* = nullptr, GameView* = nullptr);
	void OnButtonClick(int);
	void setImageRegeneration(float*){}
	void setState(STATE);
	void update(){ if (state == SHOW && isNeedUpdate) { SF_Window->Update(0.f); isNeedUpdate = false; } }
};
MainMenu::MainMenu(Vector2f _position, std::list <Entity*>* _entities, SF_Desk* _sf_desktop, GameView* _view) :SFGUI_Entity()
{
	type = SF_Desk::SFGUI_Type::headMenu;
	entities = _entities;
	sf_desktop = _sf_desktop;
	view = _view;

	SF_Window = sfg::Window::Create();
	SF_Window->SetStyle(sfg::Window::NO_STYLE);
	SF_Window->SetPosition(_position);
	SF_Box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 3.f);

	auto menuLabel = sfg::Label::Create(L"Menu");
	menuButton[0].create(skillType::noImage, false, "Save");
	menuButton[1].create(skillType::noImage, false, "Load");
	menuButton[2].create(skillType::noImage, false, "Options");
	menuButton[3].create(skillType::noImage, false, "Help");
	menuButton[4].create(skillType::noImage, false, "Quite");
	menuButton[5].create(skillType::noImage, false, "Resume");

	SF_Box->Pack(menuLabel);
	for (int i = 0; i < 6; ++i)
	{
		SF_Box->Pack(menuButton[i].getButton());
		menuButton[i].getButton()->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&MainMenu::OnButtonClick, this, i));
	}

	SF_Window->Add(SF_Box);

	entities->push_back(new SFGUI_Picture("images/missionbg.jpg", "mainMenuPicture", this, view));
}
void MainMenu::OnButtonClick(int _buttonNum)
{
	switch (_buttonNum)
	{
	case 2:
		sf_desktop->showSFGUI(SF_Desk::SFGUI_Type::optionMenu);
		break;
	case 5:
	{
		sf_desktop->showSFGUI(SF_Desk::SFGUI_Type::mainFooter);
		break;
	}
	default:
		break;
	}
}
void MainMenu::setState(STATE _state)
{
	state = _state;
	if (state == HIDE)
	{
		SF_Window->Show(false);
		for (auto it = entities->begin(); it != entities->end();)
			if ((*it)->getType() == Entity::entityType::picture && (*it)->getName() == "mainMenuPicture")
			{
				delete *it;
				it = entities->erase(it);
			}
			else ++it;
	}
	else if (state == SHOW)
	{
		SF_Window->Show(true);
		SF_Window->SetState(sfg::Widget::State::NORMAL);
		entities->push_back(new SFGUI_Picture("images/missionbg.jpg", "mainMenuPicture", this, view));
	}
	else if (state == INACTIVE)
	{
		SF_Window->SetState(sfg::Widget::State::INSENSITIVE);
	}
}

class SkillTree :public SFGUI_Entity
{
private:
	std::list <Entity*>* entities;
	Player* player;
	SF_Button skillButton[20];
	std::shared_ptr<sfg::Fixed> fixed[20];
	std::shared_ptr<sfg::Box> buttonBox[20];
	std::shared_ptr<sfg::Table> table;
	int countSkillButtons;
	Image image;
	std::shared_ptr<sfg::Image> SFGUI_Image;

	void switchButtonPosition();
public:
	SkillTree(std::list <Entity*>* = nullptr, Player* = nullptr, SF_Desk* = nullptr, GameView* = nullptr, Vector2f = { 0.f, 0.f });
	void OnButtonClick(int _buttonNum);
	void update();
	void setState(STATE _state);
};
SkillTree::SkillTree(std::list <Entity*>* _entities, Player* _player, SF_Desk* _sf_desktop, GameView* _view, Vector2f _position) :SFGUI_Entity()
{
	table = sfg::Table::Create();
	type = SF_Desk::SFGUI_Type::skillTree;
	entities = _entities;
	player = _player;
	sf_desktop = _sf_desktop;
	view = _view;
	isNeedUpdate = true;
	SF_Window = sfg::Window::Create();
	SF_Window->SetStyle(sfg::Window::NO_STYLE);
	SF_Window->SetPosition(_position);
	SFGUI_Image = sfg::Image::Create();

	countSkillButtons = 0;
	for (auto it = player->getProperties().skills.skillLevel.begin(); it != player->getProperties().skills.skillLevel.end(); ++it)
	{
		skillButton[countSkillButtons].create((*it)->type);
		fixed[countSkillButtons] = sfg::Fixed::Create();
		buttonBox[countSkillButtons] = sfg::Box::Create();
		buttonBox[countSkillButtons]->SetRequisition(Vector2f(150.f, 100.f));
		if ((*it)->level == 0)
		{
			skillButton[countSkillButtons].update(1, 0);
			skillButton[countSkillButtons].getButton()->SetState(sfg::Widget::State::INSENSITIVE);
		}
		fixed[countSkillButtons]->Put(skillButton[countSkillButtons].getButton(), Vector2f(0.f, 0.f));
		buttonBox[countSkillButtons]->Pack(fixed[countSkillButtons], true, true);
		switchButtonPosition();
		countSkillButtons++;
	}
	skillButton[countSkillButtons].create(noImage, false, "return");
	fixed[countSkillButtons] = sfg::Fixed::Create();
	buttonBox[countSkillButtons] = sfg::Box::Create();
	fixed[countSkillButtons]->Put(skillButton[countSkillButtons].getButton(), Vector2f(0.f, 0.f));
	buttonBox[countSkillButtons]->Pack(fixed[countSkillButtons], true, true);
	table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(2, 5, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));

	SF_Window->Add(table);

	for (int i = 0; i <= countSkillButtons; i++)
	{
		skillButton[i].getButton()->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&SkillTree::OnButtonClick, this, i));
	}

	entities->push_back(new SFGUI_Picture("images/missionbg.jpg", "skillTreePicture", this, view));
}
void SkillTree::switchButtonPosition()
{
	switch (skillButton[countSkillButtons].getSkill())
	{
	case skillType::fireBall:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(0, 0, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::fireBurst:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(0, 3, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::purpleBall:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(0, 1, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::webThrow:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(0, 2, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::strength:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(2, 3, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::silence:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(1, 2, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::horror:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(1, 1, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::gainArmor:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(2, 2, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::lance:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(2, 1, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	case skillType::sword:
		table->Attach(buttonBox[countSkillButtons], Rect<Uint32>(2, 0, 1, 1), sfg::Table::EXPAND, sfg::Table::EXPAND, Vector2f(0.f, 0.f));
		break;
	default:
		break;
	}
}
void SkillTree::update()
{
	if (state == SHOW && isNeedUpdate)
	{
		isNeedUpdate = false;
		for (size_t i = 0; i <= countSkillButtons; i++)
		{
			for (auto it = player->getProperties().skills.skillLevel.begin(); it != player->getProperties().skills.skillLevel.end(); ++it)
			{
				if ((*it)->type == skillButton[i].getSkill())
				{
					skillButton[i].getButton()->SetState(sfg::Widget::State::INSENSITIVE);
					if ((*it)->level == 0)skillButton[i].update(1, 0);
					else skillButton[i].update(0, 0);
					break;
				}
			}
		}
		SF_Window->Update(0.f);
	}
}
void SkillTree::OnButtonClick(int _buttonNum)
{
	if (_buttonNum == countSkillButtons)
		sf_desktop->showSFGUI(SF_Desk::SFGUI_Type::mainFooter);
}
void SkillTree::setState(STATE _state)
{
	state = _state;
	if (state == HIDE)
	{
		SF_Window->Show(false);
		for (auto it = entities->begin(); it != entities->end();)
			if ((*it)->getType() == Entity::entityType::picture && (*it)->getName() == "skillTreePicture")
			{
				delete *it;
				it = entities->erase(it);
			}
			else ++it;
	}
	else if (state == SHOW)
	{
		SF_Window->Show(true);
		SF_Window->SetState(sfg::Widget::State::NORMAL);
		entities->push_back(new SFGUI_Picture("images/missionbg.jpg", "skillTreePicture", this, view));
	}
	else if (state == INACTIVE)
	{
		SF_Window->SetState(sfg::Widget::State::INSENSITIVE);
	}
}

SF_Desk::SF_Desk(std::list<std::shared_ptr<SFGUI_Entity>>* _sf_entities, std::list <Entity*>* _entities, Player* _player, GameView* _view, Vector2f _requesition)
{
	create(_sf_entities, _entities, _player, _view, _requesition);
}
void SF_Desk::create(std::list<std::shared_ptr<SFGUI_Entity>>* _sf_entities, std::list <Entity*>* _entities, Player* _player, GameView* _view, Vector2f _requesition)
{
	sf_entities = _sf_entities;
	entities = _entities;
	player = _player;
	view = _view;
	requesition = _requesition;
}
SF_Desk::~SF_Desk(){}
void SF_Desk::showSFGUI(SFGUI_Type _type)
{
	bool isNeedLoad = true;
	for (auto it = sf_entities->begin(); it != sf_entities->end(); ++it)
	{
		if ((*it)->getType() == _type)
		{
			isNeedLoad = false;
			(*it)->setUpdate(true);
			(*it)->setState(SFGUI_Entity::STATE::SHOW);
		}
		else if (_type == skillsFooter) (*it)->setState(SFGUI_Entity::STATE::INACTIVE);
		else (*it)->setState(SFGUI_Entity::STATE::HIDE);
	}
	if (isNeedLoad)
		switchSFGUI_Entity(_type);
}
void SF_Desk::switchSFGUI_Entity(SFGUI_Type _type)
{
	switch (_type)
	{
	case mainFooter:
		sf_entities->push_back(std::make_shared<Footer>(player, Vector2f(requesition.x - 375.f - (375.f / 2.f), requesition.y - 46.f - 24.f), this));
		break;
	case skillsFooter:
		sf_entities->push_back(std::make_shared<SkillFooter>(player, Vector2f(requesition.x - 575.f - (375.f / 2.f), requesition.y - 246.f - 24.f), this, sf_entities));
		break;
	case headMenu:
		sf_entities->push_back(std::make_shared<MainMenu>(Vector2f(requesition.x / 2.f, requesition.y / 2), entities, this, view));
		break;
	case optionMenu:
		sf_entities->push_back(std::make_shared<OptionMenu>(this));
		break;
	case skillTree:
		sf_entities->push_back(std::make_shared<SkillTree>(entities, player, this, view));
		break;
	default:
		break;
	}
	SF_Desktop.Add(sf_entities->back()->getWindow());
}
void SF_Desk::update()
{
	for (auto it = sf_entities->begin(); it != sf_entities->end(); ++it)
	{
		(*it)->update();
	}
	SF_Desktop.Update(0.f);
}


class GameInit
{
private:
	sfg::SFGUI sfgui;
	float time;
	bool isGamePause,
		isSwitchEvent;
	RenderWindow window;
	Clock clock;
	GameView* view;
	tmx::MapLoader* IsoMap;
	//Level lvl;
	Event event;
	Player* Player1;
	SF_Desk sfgui_desktop;
	Cursor* cursor;
	std::vector<tmx::MapObject> collisionObj;
	std::list <Entity*>* entities;
	std::list<std::shared_ptr<SFGUI_Entity>>* sf_entities;

	void loadEntities();
	void switchEvent();
	void drowEntities();
	void endGame();

public:
	GameInit();
	void run();
};
GameInit::GameInit()
{
	isGamePause = false;
	isSwitchEvent = true;
	window.create(VideoMode(800, 600), "Game Tkir");
	window.setMouseCursorVisible(false);
	window.resetGLStates();

	entities = new std::list <Entity*>;
	sf_entities = new std::list<std::shared_ptr<SFGUI_Entity>>;
	cursor = new Cursor(entities);

	IsoMap = new tmx::MapLoader("maps/");	
	IsoMap->Load("isometric_grass_and_water.tmx");
	for (auto layer = IsoMap->GetLayers().begin(); layer != IsoMap->GetLayers().end(); ++layer)
	{
		if (layer->name == "Collision")
		{
			for (auto object = layer->objects.begin(); object != layer->objects.end(); ++object)
			{
				if (object->GetType() == "player")
				{
					Player1 = new Player("images/SoMHero.png", object->GetName(), entities, &collisionObj, object->GetPosition(), cursor);
					entities->push_back(Player1);
				}
				else if (object->GetType() == "enemy")
					entities->push_front(new Enemy("images/enemy_tileset.png", object->GetName(), entities, &collisionObj, object->GetPosition()));
				else collisionObj.push_back(*object);
			}
		}
	}

	view = new GameView(Player1, Vector2f(Player1->getPosition().x, Player1->getPosition().y));
	sfgui_desktop.create(sf_entities, entities, Player1, view, Vector2f{ (float)window.getSize().x, (float)window.getSize().y });
	sfgui_desktop.showSFGUI(SF_Desk::SFGUI_Type::mainFooter);
}
void GameInit::run()
{
	while (window.isOpen())
	{
		if (!Load_Singleton::getInstance()->getMusic()->getStatus())
			Load_Singleton::getInstance()->takeMusic()->play();

		time = (!isGamePause) ? (float)clock.getElapsedTime().asMicroseconds() : time = 0;
		clock.restart();
		time /= 800.0f;

		while (window.pollEvent(event))
		{

			if (event.type == Event::MouseMoved)
			{
				cursor->update(window.mapPixelToCoords(Mouse::getPosition(window)));
				for (auto it = sf_entities->begin(); it != sf_entities->end(); ++it)
				{
					if (!(*it)->getState() == SFGUI_Entity::STATE::HIDE)
						if ((*it)->getRect().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
						{
							window.setMouseCursorVisible(true);
							isSwitchEvent = false;
							break;
						}
						else
						{
							window.setMouseCursorVisible(false);
							isSwitchEvent = true;
						}
				}
			}
			if (isSwitchEvent) switchEvent();
			else sfgui_desktop.getDesk().HandleEvent(event);
		}
		view->update();
		window.setView(view->getView());
		window.clear();
		window.draw(*IsoMap);
		IsoMap->Draw(window, tmx::MapLayer::Debug);

		drowEntities();

		sfgui_desktop.update();
		sfgui.Display(window);

		window.display();
	}
}
void GameInit::switchEvent()
{
	if (event.type == Event::MouseButtonPressed)
	{
		if (!isGamePause)
		{
			Player1->plEvent(event, window.mapPixelToCoords(Mouse::getPosition(window)));
		}
	}

	else if (event.type == Event::KeyPressed)
	{
		if (event.key.code == Keyboard::Escape)
		{
			isGamePause = !isGamePause;
			if (isGamePause)
			{
				window.setMouseCursorVisible(true);
				sfgui_desktop.showSFGUI(SF_Desk::SFGUI_Type::headMenu);
			}
			else
			{
				window.setMouseCursorVisible(false);
				sfgui_desktop.showSFGUI(SF_Desk::SFGUI_Type::mainFooter);
			}
		}
		else if (!isGamePause && (event.key.code == Keyboard::Num1 ||
			event.key.code == Keyboard::Num2 ||
			event.key.code == Keyboard::Num3 ||
			event.key.code == Keyboard::Num4 ||
			event.key.code == Keyboard::Num5))
		{
			Player1->plEvent(event, window.mapPixelToCoords(Mouse::getPosition(window)));
		}
		else if (event.key.code == Keyboard::S)
			sfgui_desktop.showSFGUI(SF_Desk::SFGUI_Type::skillTree);
	}
	else if (event.type == Event::Closed)endGame();
}
void GameInit::drowEntities()
{
	for (auto it = entities->begin(); it != entities->end();)
	{
		if (isGamePause)
		{
			if ((*it)->getType() == Entity::entityType::pausePicture)
			{
				(*it)->update(time);
			}
		}
		else
		{
			if ((*it)->getType() == Entity::entityType::pausePicture)continue;
			(*it)->update(time);
		}

		if ((*it)->getType() != Entity::entityType::lettering)window.draw((*it)->getSprite());
		else window.draw((*it)->getText());

		if ((*it)->getLife() == false)
		{
			delete *it;
			it = entities->erase(it);
		}
		else ++it;
	}

	window.draw(cursor->getSprite());
}
void GameInit::endGame()
{
	for (auto it = entities->begin(); it != entities->end();)
	{
		delete *it;
		it = entities->erase(it);
	}
	delete entities;
	for (auto it = sf_entities->begin(); it != sf_entities->end();)
	{
		it = sf_entities->erase(it);
	}
	delete sf_entities;
	delete IsoMap;
	delete cursor;
	delete view;
	window.close();
}

int main()
{
	srand(time(NULL));
	GameInit tkirGame;
	tkirGame.run();

	return EXIT_SUCCESS;
}
