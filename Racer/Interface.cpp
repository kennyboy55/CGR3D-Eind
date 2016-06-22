#include "Interface.h"
#include <GL/freeglut.h>
#include "Racer.h"

#include <string>

#include "Player.h"
#include "Util.h"

Interface::Interface()
{
}

Interface::Interface(int stars)
{
	this->stars = stars;
}


Interface::~Interface()
{
}

void Interface::draw()
{
	Player* player = Player::getInstance();

	//Switch view to Ortho
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1000, 1000, 0, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	
	//Draw interface

	//Stars bar
	glBegin(GL_QUADS);
	glColor4f(0, 0, 0, 1.0);
	glVertex2f(250, 980);
	glVertex2f(250, 965);
	glVertex2f(750, 965);
	glVertex2f(750, 980);
	glEnd();

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 0.1f, 1.0);
	glVertex2f(250, 980);
	glVertex2f(250, 965);

	glColor4f(1.0f, 1.0f, 0.5f, 1.0);
	glVertex2f(250 + ((player->stars / (float)stars) * 500), 965);
	glVertex2f(250 + ((player->stars / (float)stars) * 500), 980);
	glEnd();

	//Text: level
	glColor4f(1.0f, 1.0f, 0.1f, 1.0);
	Util::glutBitmapString("Stars: " + std::to_string(player->stars) + " / " + std::to_string(stars), 480, 940);

	//Text: weapons
	//Util::glutBitmapString(player->leftWeapon->name, 850, 900);
	//Util::glutBitmapString(player->rightWeapon->name, 10, 900);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void Interface::update(float deltaTime)
{

}