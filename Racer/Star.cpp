#include "Star.h"
#include "Model.h"


Star::Star(Vec3f position)
{
	model = Model::load("resources/models/star/I_star.obj");
	scale = 0.2;
	this->position = position;
	rotVal = 0;
}


Star::~Star()
{
}

void Star::update(float deltaTime)
{
	rotation.y += deltaTime * 100.0f;
}
