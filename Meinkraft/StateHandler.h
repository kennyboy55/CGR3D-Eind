#pragma once
#include "State.h"

class StateHandler
{
public:
	~StateHandler();

	static StateHandler* getInstance(void);

	enum EState { WORLD, MENU };

	void changeState(EState newState);

	void draw(void);
	void update(float deltaTime);
private:
	StateHandler();
	static StateHandler* instance;

	State* CurrentState;
	EState CState;
	bool available;
};

