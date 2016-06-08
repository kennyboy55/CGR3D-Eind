#pragma once
#include "State.h"

class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void init(void);
	void exit(void);

	void draw(void);
	void update(float deltaTime);
};

