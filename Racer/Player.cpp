#define _USE_MATH_DEFINES
#include <cmath>
#include "Player.h"
#include <GL/freeglut.h>

#include <string>
#include <iostream>
#include <fstream>

Player* Player::instance = NULL;

Player::Player()
{
	speed = 10;
	stars = 0;
}

Player* Player::getInstance()
{
	if (instance == nullptr)
		instance = new Player();

	return instance;
}

void Player::init()
{
	instance = new Player();
}

void Player::setObject(LevelObject * obj)
{
	kart = obj;
	kart->position = position;
}

Player::~Player()
{
}

void Player::setCamera()
{
	gluLookAt(0, 4, -6, 0, 0, 1, 0, 0.1f, 0);

	glRotatef(rotation.x, 1, 0, 0);
	glRotatef(rotation.y, 0, 1, 0);
	//glTranslatef(-position.x, -position.y, -position.z);
}

void Player::setPosition(float angle, float fac, bool height)
{	
	if (height)
		position.y += angle*fac;
	else
	{
		position.x -= (float)cos((rotation.y + angle) / 180 * M_PI) * fac;
		position.z -= (float)sin((rotation.y + angle) / 180 * M_PI) * fac;
	}

	kart->position = position;
	kart->rotation = Vec3f(0, -rotation.y, 0);
}

void Player::draw() {

	glTranslatef(-position.x, -position.y, -position.z);

	if (kart != nullptr)
		kart->draw();
}