#include <GL/freeglut.h>

#include "Racer.h"
#include <stdio.h>
#include "Vector.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdlib.h>
#include <time.h>
#include "Cursor.h"

void configureOpenGL(void);

Racer* app;

bool justMoved = false;

int main(int argc, char* argv[])
{
	app = new Racer();
	glutInit(&argc, argv);

	srand (time(NULL));

	configureOpenGL();

	app->init();

	glutDisplayFunc([]() { app->draw(); } );
	glutIdleFunc([]() { app->update(); } );
	glutReshapeFunc([](int w, int h) { Racer::width = w; Racer::height = h; glViewport(0, 0, w, h); });

	//Keyboard
	glutKeyboardFunc([](unsigned char c, int, int) { app->keyboardState.keys[c] = true; });
	glutKeyboardUpFunc([](unsigned char c, int, int) { app->keyboardState.keys[c] = false; });
	glutSpecialFunc([](int c, int, int) { app->keyboardState.special[c] = true; });
	glutSpecialUpFunc([](int c, int, int) { app->keyboardState.special[c] = false; });
	
	auto mousemotion = [](int x, int y)
	{
		if (justMoved)
		{
			justMoved = false;
			return;
		}
		int dx = x - app->width / 2;
		int dy = y - app->height / 2;
		if ((dx != 0 || dy != 0) && abs(dx) < 400 && abs(dy) < 400)
		{
			app->mouseOffset = app->mouseOffset + Vec2f(dx, dy);
			glutWarpPointer(app->width / 2, app->height / 2);
			justMoved = true;
		}
	};

	//Mouse
	glutPassiveMotionFunc(mousemotion);
	glutMotionFunc(mousemotion);

	auto mouseclick = [](int button, int state,
		int x, int y)
	{
		if (button == GLUT_LEFT_BUTTON)
			Cursor::getInstance()->state = state;


			//std::cout << "Left button is down" << std::endl;
	};

	glutMouseFunc(mouseclick);

	Racer::height = GLUT_WINDOW_HEIGHT;
	Racer::width = GLUT_WINDOW_WIDTH;

	glutMainLoop();

	delete app;
	return 0;
}

void configureOpenGL()
{
	//Init window and glut display mode
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1440, 900);
	//glutInitWindowPosition(glutGet(GLUT_WINDOW_WIDTH) / 2 - 800/2, glutGet(GLUT_WINDOW_HEIGHT) / 2 - 600/2);
	glutCreateWindow("Racer");	
	//glutFullScreen();


	//Depth testing
	glEnable(GL_DEPTH_TEST);

	//Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Alpha testing
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.01f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutSetCursor(GLUT_CURSOR_NONE);
}