
#include "Racer.h"
#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include "WorldHandler.h"
#include "Player.h"
#include "Cursor.h"
#include "Menu.h"
#include "Text.h"
#include "Vector.h"
#include "Button.h"

int Racer::width = 0;
int Racer::height = 0;

SoundSystem Racer::sound_system;
bool state = false;

void Racer::init()
{
	player = Player::getInstance();
	worldhandler = WorldHandler::getInstance();
	cursor = Cursor::getInstance();

	menu = new Menu();
	buildMenu();

	lastFrameTime = 0;
	state = true;

	glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
}


void Racer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw world
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, width / (float)height, 0.1f, 7500);
	//gluLookAt()
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	worldhandler->draw();
	
	if(!state)
		menu->draw();

	glutSwapBuffers();
}


void Racer::update()
{
	float frameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float deltaTime = frameTime - lastFrameTime;
	lastFrameTime = frameTime;

	if (keyboardState.keys[27] && !prevKeyboardState.keys[27])
        state = !state;

	if (state)
	{
		Player* player = Player::getInstance();

		if (keyboardState.special[GLUT_KEY_LEFT] && !prevKeyboardState.special[GLUT_KEY_LEFT])
			worldhandler->PreviousWorld();
		if (keyboardState.special[GLUT_KEY_RIGHT] && !prevKeyboardState.special[GLUT_KEY_RIGHT])
			worldhandler->NextWorld();
		if (keyboardState.keys[27])
			state = false;

		player->rotation.y += mouseOffset.x / 10.0f;
		player->rotation.x += mouseOffset.y / 10.0f;

		float speed = player->speed;

		Vec3f oldPosition = player->position;

		if (keyboardState.keys['w']) player->setPosition(270, deltaTime*speed, false);
		if (keyboardState.keys['s']) player->setPosition(90, deltaTime*speed, false);


		if (player->rotation.x > 25)
			player->rotation.x = 25;
		if (player->rotation.x < -20)
			player->rotation.x = -20;

		player->position.y = worldhandler->getHeight(player->position.x, player->position.z) + 0.1f;

		if (!worldhandler->isPlayerPositionValid())
			player->position = oldPosition;

		worldhandler->update(deltaTime);
	}	
	else
	{
		menu->update();
		cursor->update(cursor->mousePosition + mouseOffset);
	}


	mouseOffset = Vec2f(0, 0);
	prevKeyboardState = keyboardState;
	glutPostRedisplay();

	sound_system.SetListener(player->position, Vec3f(), Vec3f());
}

void Racer::buildMenu()
{
	Button* start = new Button("Resume", Vec2f(1920 / 2 - 50, 1080 / 2 - 30), 100, 50);
	auto toWorld = [](Button* b)
	{
		state = true;
	};
	start->addAction(toWorld);
	menu->AddMenuElement(start);


	Button* test = new Button("Exit", Vec2f(1920 / 2 - 50, 1080 / 2 + 30), 100, 50);
	test->addAction([](Button* b)
	{
		exit(0);
	});
	menu->AddMenuElement(test);
	Text* t = new Text("Pause", Vec2f(1920 / 2 - Util::glutTextWidth("Pause") / 2, 1080 / 2 - 75));
	t->setColor(Vec3f(255, 255, 0));
	menu->AddMenuElement(t);
}



KeyboardState::KeyboardState()
{
	memset(keys, 0, sizeof(keys));
	memset(special, 0, sizeof(special));
}
