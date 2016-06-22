#include "World.h"
#include <GL/freeglut.h>
#include "Entity.h"
#include "json.h"
#include "Model.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <cstdlib>
#include "WorldHandler.h"
#include "LevelObject.h"

World::World(const std::string &fileName)
{
	nextworld = false;

	//Store player instance
	player = Player::getInstance();

	//Open world json file
	std::ifstream file(fileName);
	if(!file.is_open())
		std::cout<<"Error, can't open world file - " << fileName << "\n";

	json::Value v = json::readJson(file);
	file.close();

	//Check file
	if(v["world"].isNull() || v["world"]["heightmap"].isNull() || v["world"]["skybox"].isNull())
		std::cout << "Invalid world file: world - " << fileName << "\n";
	if (v["world"]["object-templates"].isNull())
		std::cout << "Invalid world file: object templates - " << fileName << "\n";
	if (v["player"].isNull() || v["player"]["startposition"].isNull() || v["player"]["kart"].isNull())
		std::cout << "Invalid world file: player - " << fileName << "\n";
	if (v["objects"].isNull())
		std::cout << "Invalid world file: objects - " << fileName << "\n";

	//Load object templates
	for (auto objt : v["world"]["object-templates"])
	{
		//collision
		bool cancollide = true;
		if (!objt["collision"].isNull())
			cancollide = objt["collision"].asBool();

		float scaleot = 1;
		if (!objt["scale"].isNull())
			scaleot = objt["scale"].asFloat();

		Vec3f rotationot = Vec3f(0,0,0);
		if (!objt["rot"].isNull())
			rotationot = Vec3f(objt["rot"][0].asFloat(), objt["rot"][3].asFloat(), objt["rot"][2].asFloat());

		objecttemplates.push_back(ObjectTemplate(objt["file"].asString(), objt["color"].asInt(), cancollide, scaleot, rotationot));
	}

	//Generate heightmap for this world
	heightmap = new HeightMap(v["world"]["heightmap"].asString(), this);

	//Load skybox
	skybox = new Skybox(7500.0f, v["world"]["skybox"].asString());
	skybox->init();

	//Map different texture to heightmap if available
	if(!v["world"]["texture"].isNull())
		heightmap->SetTexture(v["world"]["texture"].asString());

	//Set player starting position
	player->position.x = v["player"]["startposition"][0].asFloat();
	player->position.z = v["player"]["startposition"][2].asFloat();
	player->position.y = heightmap->GetHeight(player->position.x, player->position.z);

	float pscale = 1.0f;
	if(!v["player"]["kart"]["scale"].isNull())
		pscale = v["player"]["kart"]["scale"].asFloat();


	player->setObject(new LevelObject(v["player"]["kart"]["file"].asString(), Vec3f(), Vec3f(), pscale, false));

	//Create the interface
	starsCount = stars.size();
	interface = new Interface(stars.size());

	//Load and place objects into world
	for (auto object : v["objects"])
	{
		//Collision
		bool hasCollision = true;
		if (!object["collide"].isNull())
			hasCollision = object["collide"].asBool();

		//Rotation
		Vec3f rotation(0, 0, 0);
		if(!object["rot"].isNull())
			rotation = Vec3f(object["rot"][0].asFloat(), object["rot"][1].asFloat(), object["rot"][2].asFloat());

		//Scale
		float scale = 1;
		if (!object["scale"].isNull())
			scale = object["scale"].asFloat();
		
		//Position
		if (object["pos"].isNull())
			std::cout << "Invalid world file: objects pos - " << fileName << "\n";
		
		//File
		if (object["file"].isNull())
			std::cout << "Invalid world file: objects file - " << fileName << "\n";

		//Create
		Vec3f position(object["pos"][0].asFloat(), object["pos"][1].asFloat(), object["pos"][2].asFloat());		
		position.y = getHeight(position.x, position.z);

		entities.push_back(new LevelObject(object["file"].asString(), position, rotation, scale, hasCollision));
	}

	if (!v["world"]["music"].isNull())
	{
		sound_id = Racer::GetSoundSystem().LoadSound(v["world"]["music"].asString().c_str(), true);
		music = Racer::GetSoundSystem().GetSound(sound_id);
	}

	star_sound_id = Racer::GetSoundSystem().LoadSound("resources/sounds/Crystal.wav", false);
	starPickup = Racer::GetSoundSystem().GetSound(star_sound_id);
}

World::~World()
{
	delete heightmap;
	music->Stop();
	starPickup->Stop();
	Racer::GetSoundSystem().UnloadSound(sound_id);
	Racer::GetSoundSystem().UnloadSound(star_sound_id);
	delete skybox;	
}

ObjectTemplate World::getObjectFromValue(int val)
{
	for (auto i : objecttemplates)
	{
		if (i.color == val)
			return i;
	}

	return objecttemplates[0];
}

float World::getHeight(float x, float y)
{
	return heightmap->GetHeight(x, y);
}

void World::draw()
{
	player->setCamera();

	float lightPosition[4] = { 0, 2, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	GLfloat lightAmbient[] = { 0.05, 0.05, 0.05, 0 };
	GLfloat light_diffuse[] = { 0.9, 0.9, 0.9, 0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);

	GLfloat mat_specular[] = { 0.15, 0.15, 0.15, 0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	skybox->draw();
	player->draw();

	heightmap->Draw();

	for (auto &entity : entities)
		entity->draw();

	for (auto &star : stars)
		star->draw();

	interface->draw();
}

void World::update(float elapsedTime)
{
	if (nextworld)
	{
		WorldHandler::getInstance()->NextWorld();
		return;
	}

	music->SetPos(player->position, Vec3f());

	if (music->IsPlaying() ==  false)
	{
		music->Play();
	}

	for (auto &entity : entities)
		entity->update(elapsedTime);

	bool remove = false;
	int removeindex = 0;

	for (auto &star : stars)
	{
		star->update(elapsedTime);

		if (star->inObject(player->position))
		{
			remove = true;
			starPickup->Stop();
			starPickup->SetPos(Vec3f(), star->position);
			starPickup->Play();
			player->stars++;
			continue;
		}

		if (!remove)
			removeindex++;
	}

	if (remove)
	{
		delete stars[removeindex];
		stars.erase(stars.begin() + removeindex);
	}

	if (player->stars == starsCount)
		nextworld = true;

	interface->update(elapsedTime);
		
}

void World::addLevelObject(LevelObject* obj)
{
	entities.push_back(obj);
}

void World::addStar(Star * star)
{
	stars.push_back(star);
}

bool World::isPlayerPositionValid()
{
	for (auto &e : entities)
	{
		if (e->canCollide && e->inObject(player->position))
		{
			e->collide();
			return false;
		}
	}
	return true;
}

