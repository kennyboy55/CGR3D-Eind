#pragma once
class State
{
public:
	State();
	~State();

	virtual void init(void) = 0;
	virtual void exit(void) = 0;

	virtual void draw(void) = 0;
	virtual void update(float deltaTime) = 0;
};

