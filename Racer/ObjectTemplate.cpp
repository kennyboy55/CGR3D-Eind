#include "ObjectTemplate.h"

ObjectTemplate::ObjectTemplate(std::string file, int color, bool collide, float scale, Vec3f rotation)
{
	this->file = file;
	this->color = color;
	this->canCollide = collide;
	this->scale = scale;
	this->rotation = rotation;
}

ObjectTemplate::~ObjectTemplate()
{
}
