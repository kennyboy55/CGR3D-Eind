#include "WorldState.h"



WorldState::WorldState()
{
}


WorldState::~WorldState()
{
}

void WorldState::init(void)
{
	world = new World();
}

void WorldState::exit(void)
{
	delete world;
}

void WorldState::draw(void)
{
	world->draw();
}

void WorldState::update(float deltaTime)
{
	world->update(deltaTime);
}
