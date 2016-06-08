#pragma once

class Player;
class StateHandler;
#include "Vector.h"

class KeyboardState
{
public:
	bool keys[256];
	bool special[256];
	bool control, shift, alt;

	KeyboardState();
};

class Meinkraft
{
public:
	void init();
	void draw();
	void update();

	Player* player;
	StateHandler* statehandler;

	static int width, height;
	KeyboardState keyboardState;
	KeyboardState prevKeyboardState;

	Vec2f mouseOffset;
	Vec2f mousePosition;

	float lastFrameTime;
};