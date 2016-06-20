#include "StateHandler.h"
#include "MenuState.h"
#include "WorldState.h"

StateHandler* StateHandler::instance = nullptr;

StateHandler::StateHandler()
{
	available = false;
	CState = WORLD;
	CurrentState = new WorldState();
	CurrentState->init();
	available = true;
}

StateHandler::~StateHandler()
{
	delete CurrentState;
}

StateHandler* StateHandler::getInstance()
{
	if (instance == nullptr)
		instance = new StateHandler();

	return instance;
}

void StateHandler::update(float deltaTime)
{
	if(available)
		CurrentState->update(deltaTime);
}
void StateHandler::draw()
{
	if(available)
		CurrentState->draw();
}

void StateHandler::changeState(EState newState)
{
	if (CState == newState)
		return;

	available = false;

	CurrentState->exit();

	switch (newState)
	{
	case WORLD:
		CurrentState = new WorldState();
	case MENU:
		CurrentState = new MenuState();
	}

	CState = newState;
	CurrentState->init();

	available = true;
}
