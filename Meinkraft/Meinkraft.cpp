
#include "Meinkraft.h"
#include <cmath>
#include <cstring>
#include "Player.h"
#include "StateHandler.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "stb_image.h"
#include "stb_perlin.h"

int Meinkraft::width = 0;
int Meinkraft::height = 0;
GLuint Meinkraft::texture = NULL;

void Meinkraft::loadTexture(void)
{
	//Load Textures :: blocks
	int img_width, img_height, bpp;
	unsigned char* imgData = stbi_load("/resources/terrain.png", &img_width, &img_height, &bpp, 4);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D,
		0,				  //level
		GL_RGBA,		  //internal format
		img_width,		  //width
		img_height,		  //height
		0,				  //border
		GL_RGBA,		  //data format
		GL_UNSIGNED_BYTE, //data type
		imgData);		  //data

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(imgData);
}

void Meinkraft::init()
{
	player = Player::getInstance();
	statehandler = StateHandler::getInstance();
	//cursor = Cursor::getInstance();

	loadTexture();

	lastFrameTime = 0;

	glClearColor(0.7f, 0.7f, 1.0f, 1.0f);

	mousePosition = Vec2f(width / 2, height / 2);
}


void Meinkraft::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw world
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, width / (float)height, 0.1f, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	statehandler->draw();

	//cursor->draw();

	glutSwapBuffers();
}


void Meinkraft::update()
{
	float frameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float deltaTime = frameTime - lastFrameTime;
	lastFrameTime = frameTime;

	if (keyboardState.keys[27])
		exit(0);

	Player* player = Player::getInstance();

	player->rotation.y += mouseOffset.x / 10.0f;
	player->rotation.x += mouseOffset.y / 10.0f;
	if (player->rotation.x > 90)
		player->rotation.x = 90;
	if (player->rotation.x < -90)
		player->rotation.x = -90;

	float speed = 10;

	Vec3f oldPosition = player->position;
	if (keyboardState.keys['a']) player->setPosition(0, deltaTime*speed, false);
	if (keyboardState.keys['d']) player->setPosition(180, deltaTime*speed, false);
	if (keyboardState.keys['w']) player->setPosition(90, deltaTime*speed, false);
	if (keyboardState.keys['s']) player->setPosition(270, deltaTime*speed, false);
	if (keyboardState.keys['q']) player->setPosition(1, deltaTime*speed, true);
	if (keyboardState.keys['e']) player->setPosition(-1, deltaTime*speed, true);

	//if (!worldhandler->isPlayerPositionValid())
	//	player->position = oldPosition;

	//player->position.y = worldhandler->getHeight(player->position.x, player->position.z) + 1.7f;

	statehandler->update(deltaTime);

	mousePosition = mousePosition + mouseOffset;
	//cursor->update(mousePosition);

	mouseOffset = Vec2f(0, 0);
	prevKeyboardState = keyboardState;
	glutPostRedisplay();
}

KeyboardState::KeyboardState()
{
	memset(keys, 0, sizeof(keys));
	memset(special, 0, sizeof(special));
}
