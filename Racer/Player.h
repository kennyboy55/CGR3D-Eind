#pragma once
#include "Vector.h"
#include "json.h"
#include "LevelObject.h"

#include <vector>

class Player
{
private:
	static Player* instance;
	
	LevelObject* kart;

public:
	Player();
	~Player();

	void setCamera();
	void setPosition(float angle, float fac, bool height);
	void draw(void);

	static Player* getInstance(void);
	static void init(void);

	void setObject(LevelObject * obj);

	Vec3f position;
	Vec2f rotation;

	float speed;
	int stars;
};