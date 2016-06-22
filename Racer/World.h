#pragma once

#include <vector>
#include "HeightMap.h"
#include "Player.h"
#include "LevelObject.h"
#include "Interface.h"
#include "Skybox.h"
#include "Racer.h"
#include "ObjectTemplate.h"
#include "Star.h"

class Entity;

class World
{
private:
	std::vector<ObjectTemplate> objecttemplates;
	Sound* music;
	Sound* starPickup;

	Player* player;
	HeightMap* heightmap;
	Interface* interface;
	Skybox* skybox;

	bool nextworld;
	
	int sound_id;
	int star_sound_id;
	int starsCount;

	std::vector<Entity*> entities;
	std::vector<Star*> stars;
public:
	World(const std::string &fileName);
	~World();

	void draw();
	void update(float elapsedTime);
	bool isPlayerPositionValid();
	float getHeight(float x, float y);
	void addLevelObject(LevelObject* obj);
	void addStar(Star* star);
	ObjectTemplate getObjectFromValue(int i);
	
};

