
#include "ToTheMoon.h"

ToTheMoon *game;

//global variables for window
int width = 1200;
int height = 600;
float ratio = 1.0f;

//functions to create menus for the game and modify display
//create a menu listener and pass in menu option
void menuListener(int option) {
	//check passed in option
	switch (option) {
	case 1:
		if (game->showFog) {
			game->showFog = false;
		}
		else
			game->showFog = true;
		break;
	case 2:
		if (game->showBoundingBox) {
			game->showBoundingBox = false;
		}
		else
			game->showBoundingBox = true;
		break;
	case 3:
		if (game->showSkyBox) {
			game->showSkyBox = false;
		}
		else
			game->showSkyBox = true;
		break;
	case 4:
		if (game->editorMode) {
			game->editorMode = false;

			//restore the player mode camera
			game->loadPlayerCamera();
		}
		else {
			game->editorMode = true;
			//store the player camera view
			game->setEditorCamera();
		}
		break;
	}
	glutPostRedisplay();
}

//menu function to create menus to swap between showing
//features
void createMenus() {

	//create fog menu
	int fogMenu = glutCreateMenu(menuListener);
	glutAddMenuEntry("Enable/Disable", 1);

	//create bounding box menu
	int boundBoxMenu = glutCreateMenu(menuListener);
	glutAddMenuEntry("Enable/Disable", 2);

	//create skybox menu
	int skyBoxMenu = glutCreateMenu(menuListener);
	glutAddMenuEntry("Enable/Disable", 3);

	//create flat plain menu
	int editorModeMenu = glutCreateMenu(menuListener);
	glutAddMenuEntry("Enable/Disable", 4);

	//create main menu
	int mainMenu = glutCreateMenu(menuListener);
	glutAddSubMenu("Fog", fogMenu);
	glutAddSubMenu("AABB", boundBoxMenu);
	glutAddSubMenu("Skybox", skyBoxMenu);
	glutAddSubMenu("Editor Mode", editorModeMenu);

	//attatch menu to right mouse button
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

//create callback functions
void idle() {
	game->gameLoop();
}

void display() {
	game->render();
}

void gameKeyboardDown(unsigned char key, int x, int y) {
	game->getKeyboard()->readKeyboard(key, x, y, true);
}

void gameKeyboardUp(unsigned char key, int x, int y) {
	game->getKeyboard()->readKeyboard(key, x, y, false);
}

void gameInit() {
	game->init();
	createMenus();
}

void gameMouse(int button, int state, int x, int y) {
	game->mouse(button, state, x, y);
}

void gameMotion(int x, int y) {
	if (game->editorMode) {
		game->motion(x, y);
	}

	else {
		//do nothing
	}
}

// main
void main(int argc, char* argv[]) {
	game = new ToTheMoon();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Textures");
	gameInit();
	//glutReshapeFunc(gameReshape);	Come back at a later time
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(gameMouse);
	glutMotionFunc(gameMotion);
	glutKeyboardFunc(gameKeyboardDown);
	glutKeyboardUpFunc(gameKeyboardUp);

	glutMainLoop();
}