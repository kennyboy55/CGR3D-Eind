#pragma once
#include "State.h"

class WorldState : public State
{
public:
	WorldState();
	~WorldState();

	void init(void);
	void exit(void);

	void draw(void);
	void update(float deltaTime);
};

