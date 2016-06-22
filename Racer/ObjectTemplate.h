#pragma once
#include <string>
#include "Vector.h"

class ObjectTemplate
{
public:
	ObjectTemplate(std::string file, int color, bool collide, float scale, Vec3f rotation);
	~ObjectTemplate();

	std::string file;

	int color;
	bool canCollide;

	float scale;
	Vec3f rotation;
};

