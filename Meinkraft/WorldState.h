#pragma once
#include "State.h"
#include "World.h"

class WorldState : public State
{
private:
	World* world;
public:
	WorldState();
	~WorldState();

	void init(void);
	void exit(void);

	void draw(void);
	void update(float deltaTime);
};

