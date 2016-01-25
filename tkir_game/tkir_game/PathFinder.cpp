#include "PathFinder.h"

object object::create(Vector2f _pos, float tileSize)
{
	Vector2f tmp = _pos;
	_pos.x = tileSize*(tmp.x - tmp.y);
	_pos.y = tileSize / 2.f * (tmp.x + tmp.y);

	point[0] = Vector2f(_pos);
	point[1] = Vector2f(_pos.x + tileSize, _pos.y + tileSize / 2.f);
	point[2] = Vector2f(_pos.x, _pos.y + tileSize);
	point[3] = Vector2f(_pos.x - tileSize, _pos.y + tileSize / 2.f);
	centerPoint = (point[0] + point[2]) / 2.f;
	furthestPoint = Helpers::Vectors::getLength(centerPoint - point[1]);

	return *this;
}


void MiniMap::create(Vector2u _mapSize, unsigned int _miniMapTyleSize, std::vector<tmx::MapObject>* _solid)
{
	solid = _solid;
	miniMapTyleSize = (float)_miniMapTyleSize;
	mapSize = _mapSize;
	countMiniMapTyles = mapSize / (unsigned int)miniMapTyleSize;
	countMiniMapTyles.x = (unsigned int)(0.5f*(float)countMiniMapTyles.x + 1);
	countMiniMapTyles.y++;

	miniMap = new object*[countMiniMapTyles.y];
	for (size_t i = 0; i < countMiniMapTyles.y; i++)
	{
		miniMap[i] = new object[countMiniMapTyles.x];
		for (size_t j = 0; j < countMiniMapTyles.x; j++)
		{
			miniMap[i][j] = obj.create(Vector2f(j, i), miniMapTyleSize);
			miniMap[i][j].isWall = collision(miniMap[i][j]);
		}
	}
}
bool MiniMap::collision(object& _mapObj)
{
	for (auto it = solid->begin(); it != solid->end(); ++it)
	{
		float distance = Helpers::Vectors::getLength(_mapObj.centerPoint - it->GetCentre());
		if (distance >(_mapObj.furthestPoint + it->GetFurthestPoint())) continue;

		for (int i = 0; i < 4; ++i)
		{
			if (contains(_mapObj, it->GetPolipoints()[i] + Vector2f(it->GetAABB().left, it->GetAABB().top)))return true;
			if (it->Contains(_mapObj.point[i]))return true;
		}
	}
	return false;
}
bool MiniMap::contains(object& _mapObj, sf::Vector2f point)
{
	unsigned int i, j;
	bool result = false;
	for (i = 0, j = 3; i < 4; j = i++)
	{
		if (((_mapObj.point[i].y > point.y) != (_mapObj.point[j].y > point.y)) &&
			(point.x < (_mapObj.point[j].x - _mapObj.point[i].x) * (point.y - _mapObj.point[i].y)
			/ (_mapObj.point[j].y - _mapObj.point[i].y) + _mapObj.point[i].x))
			result = !result;
	}
	return result;
}
Vector2i MiniMap::getMiniPositionEntyty(Vector2f _entityPosition)//returns number cell in miniMap
{
	for (int i = 0; i < countMiniMapTyles.y; ++i)
		for (int j = 0; j < countMiniMapTyles.x; ++j)
			if (contains(miniMap[i][j], _entityPosition))return Vector2i(j, i);
}


PathFinder::PathFinder(tmx::MapObject* _entityObj, MiniMap* _map, std::vector<sf::Vector2f>* _smoothPath)
{
	entityObj = _entityObj;
	map = _map;
	m_start = Vector2f(entityObj->GetAABB().left, entityObj->GetAABB().top);
	start = map->getMiniPositionEntyty(m_start);
	smoothPath = _smoothPath;
	mapSize = Vector2i(map->getMiniMapSize());
	solid = map->getSolid();
	isPathExist = true;
	distanceMap = new int*[mapSize.y];
	for (int i = 0; i < mapSize.y; ++i)
		distanceMap[i] = new int[mapSize.x];
	for (int i = 0; i < mapSize.y; ++i)
		for (int j = 0; j < mapSize.x; ++j)
			distanceMap[i][j] = ((map->getMiniMap())[i][j]).isWall ? 9999999 : -1;
}
void PathFinder::aStar(Vector2f _target)
{
	m_start = Vector2f(entityObj->GetAABB().left, entityObj->GetAABB().top);
	start = map->getMiniPositionEntyty(m_start);
	m_target = _target;
	target = map->getMiniPositionEntyty(_target);
	sf::Vector2i tmpPos, tmp2Pos;
	std::vector<sf::Vector2i> wave;
	std::vector<sf::Vector2i> oldWave;

	const sf::Vector2i dir[] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
	int nStep = 0;
	bool isGetTarget = false;
	distanceMap[start.y][start.x] = nStep;
	oldWave.push_back(start);

	while (!isGetTarget)
	{
		isPathExist = false;
		++nStep;
		wave.clear();
		for (auto it = oldWave.begin(); it != oldWave.end() && !isGetTarget; ++it)
		{
			for (int i = 0; i < 4; ++i)
			{
				tmpPos = (*it) + dir[i];
				if (tmpPos.x > -1 && tmpPos.y > -1 &&
					tmpPos.x < mapSize.x && tmpPos.y < mapSize.y &&
					distanceMap[tmpPos.y][tmpPos.x] == -1)
				{
					isPathExist = true;
					wave.push_back(tmpPos);
					distanceMap[tmpPos.y][tmpPos.x] = nStep;
				}
				if (tmpPos == target)
				{
					isGetTarget = true;
					break;
				}
			}
		}
		if (!isPathExist && !isGetTarget)return;
		oldWave = wave;
	}

	/*for (int i = 0; i < mapSize.y; ++i)
	{
	for (int j = 0; j < mapSize.x; ++j)
	std::cout << distanceMap[i][j] << "\t";
	std::cout << std::endl;
	}std::cout << std::endl;*/


	resultPath.push_back(target);
	tmpPos = tmp2Pos = target;
	bool isGetStart;
	while (distanceMap[tmpPos.y][tmpPos.x] != 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			tmpPos = tmp2Pos;
			tmpPos += dir[i];
			if (tmpPos.x > -1 && tmpPos.y > -1 &&
				tmpPos.x < mapSize.x && tmpPos.y < mapSize.y &&
				distanceMap[tmpPos.y][tmpPos.x] == distanceMap[tmp2Pos.y][tmp2Pos.x] - 1)
			{
				resultPath.push_back(tmpPos);
				tmp2Pos = tmpPos;
				break;
			}
		}
	}

	smoothing();
}
void PathFinder::smoothing()
{
	Vector2f A[4], B[4], C[4];
	bool isIntersect = false;

	for (auto it = resultPath.rbegin(); it != resultPath.rend(); ++it)
	{
		if (it == resultPath.rbegin())
		{
			for (int i = 0; i < 4; ++i)
				A[i] = B[i] = Vector2f(m_start + entityObj->GetPolipoints()[i]);
			continue;
		}
		if (it == resultPath.rend() - 1)
		{
			for (int i = 0; i < 4; ++i)
				C[i] = Vector2f(m_target + entityObj->GetPolipoints()[i]);
		}
		else
		{
			for (int i = 0; i < 4; ++i)
				C[i] = Vector2f((map->getMiniMap())[it->y][it->x].point[0] + entityObj->GetPolipoints()[i]);
		}

		for (int i = 0; i < 4 && !isIntersect; ++i)
		{
			for (auto it2 : *solid)
			{
				isIntersect = isObstacle(A[i], C[i], &it2);
				if (isIntersect) break;
			}
		}
		//std::cout << it->x << "; " << it->y << std::endl;
		if (isIntersect)
		{
			isIntersect = false;
			//std::cout << B[0].x << "; " << B[0].y << "\t\t";			

			smoothPath->push_back(B[0]);
			for (int i = 0; i < 4; ++i)
			{
				A[i] = B[i];
				B[i] = C[i];
			}
		}
		else
		{
			for (int i = 0; i < 4; ++i)
			{
				B[i] = C[i];
			}
		}

		if (it == resultPath.rend() - 1) smoothPath->push_back(C[0]);
	}
	resultPath.clear();
	for (int i = 0; i < mapSize.y; ++i)
		for (int j = 0; j < mapSize.x; ++j)
			if (distanceMap[i][j] != 9999999)distanceMap[i][j] = -1;
}
inline bool PathFinder::isObstacle(Vector2f A, Vector2f B, tmx::MapObject* solid)
{
	bool _isIntersect = false;

	for (int i = 0; i < 4; i++)
	{
		Vector2f c, d;
		if (i < 2) c = { solid->GetAABB().left + solid->GetPolipoints()[0].x, solid->GetAABB().top + solid->GetPolipoints()[0].y };
		else c = { solid->GetAABB().left + solid->GetPolipoints()[2].x, solid->GetAABB().top + solid->GetPolipoints()[2].y };
		if (i % 2) d = { solid->GetAABB().left + solid->GetPolipoints()[1].x, solid->GetAABB().top + solid->GetPolipoints()[1].y };
		else d = { solid->GetAABB().left + solid->GetPolipoints()[3].x, solid->GetAABB().top + solid->GetPolipoints()[3].y };
		_isIntersect = isLinesCross(A.x, A.y, B.x, B.y, c.x, c.y, d.x, d.y);
		if (_isIntersect)break;
	}

	return _isIntersect;
}
inline bool PathFinder::isLinesCross(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22)
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