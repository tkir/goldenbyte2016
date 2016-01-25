#pragma once
#include "include.h"

struct object
{
	Vector2f point[4];
	Vector2f centerPoint;
	float furthestPoint;
	bool isWall;

	object(){}
	object create(Vector2f position, float tileSize);

};


class MiniMap
{
public:
	object obj;
	object** miniMap;
	Vector2u mapSize,
		countMiniMapTyles;
	float miniMapTyleSize;
	std::vector<tmx::MapObject>* solid;

	void createObj(Vector2f _pos);
	bool collision(object& _miniMapObj);
	bool contains(object& _mapObj, sf::Vector2f point);

public:
	MiniMap(){}
	void create(Vector2u _mapSize, unsigned int _miniMapObjSize = 16, std::vector<tmx::MapObject>* _solid = nullptr);
	object**& getMiniMap(){ return miniMap; }
	Vector2u getMiniMapSize(){ return countMiniMapTyles; }
	std::vector<tmx::MapObject>* getSolid(){ return solid; };
	Vector2i getMiniPositionEntyty(Vector2f _entityPosition);//returns number cell in minimap
};


class PathFinder
{
private:
	MiniMap* map;
	object*** miniMap;
	int** distanceMap;
	Vector2i mapSize,
		start,
		target;
	Vector2f m_start,
		m_target;
	tmx::MapObject* entityObj;
	std::vector<sf::Vector2i> resultPath;
	std::vector<sf::Vector2f>* smoothPath;
	bool isPathExist;
	std::vector<tmx::MapObject>* solid;


	void smoothing();
	bool isObstacle(Vector2f A, Vector2f B, tmx::MapObject* _solid);
	bool isLinesCross(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22);

public:
	PathFinder(tmx::MapObject* entityObj, MiniMap* map, std::vector<sf::Vector2f>* smoothPath = nullptr);
	void aStar(Vector2f target);

};
