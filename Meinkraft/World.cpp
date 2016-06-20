#include "World.h"
#include "Block.h"


World::World()
{
}


World::~World()
{
}

void World::draw(void)
{
	Block b = Block(Vec3f(0, 0, 0), 1, 3);
	b.draw();
}

void World::update(float deltaTime)
{
}
