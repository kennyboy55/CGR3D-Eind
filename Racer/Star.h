#pragma once
#include "Entity.h"
#include "Vector.h"

class Star : public Entity
{
private:
	float rotVal;
public:
	Star(Vec3f position);
	~Star();

	void update(float deltaTime);
};

