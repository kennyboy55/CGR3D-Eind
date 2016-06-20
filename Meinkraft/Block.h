#pragma once
#include "Vector.h"

class Block
{
private:
	float size = 1;
	bool hasTopTexture = false;

	Vec2f texColsTop;
	Vec2f texRowsTop;
	Vec2f texColsSides;
	Vec2f texRowsSides;

public:
	Block(Vec3f position, int top, int sides);
	~Block();

	void draw(void);

	Vec3f position;
};

