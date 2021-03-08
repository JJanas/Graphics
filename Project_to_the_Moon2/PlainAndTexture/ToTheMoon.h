#pragma once

//include header files for game objects
#include "Globals.h"
#include "Box.h"
#include "Lane.h"
#include "Wall.h"
#include "FlatPlain.h"
#include "NewParticle.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "Skybox.h"
#include "Keyboard.h"

//define player/camera movements
#define P1_FORWARD	'w'
#define P1_LEFT	'a'
#define P1_RIGHT 'd'
#define P1_BACK	's'
#define P1_BOOST ' '

//set the framreate 
#define FRAMERATE 60

class ToTheMoon {

public:

	//create variables to control what features are displayed (default all are true)
	bool showBoundingBox = true;
	bool showFog = true;
	bool showSkyBox = true;
	bool editorMode = false;

	//contrsuctor and destructor
	ToTheMoon();
	~ToTheMoon();

	//set up init and display methods and game logic methods
	void init();
	void gameLoop();	//game loop
	void logic();		//read input and update objects
	void render();		//display objects to the screen
	void rotate_point(float angle); //rotate the scene
	void reshape(int w, int h);	//reshape the window
	void mouse(int button, int state, int x, int y);	//mouse input method
	void motion(int x, int y);	//motion with the mouse
	void renderBitmapString(float x, float y, float Z, const char *string);	//method to print text
	void setEditorCamera();	//store the Player camera values
	void loadPlayerCamera();	//place the stored player camera values into the camera
	void drawParticles();	//draw particles
	Keyboard* getKeyboard();	//get the keyboard input

	//hold game objects
private:
	//for the window sizing
	int width = 1200;
	int height = 600;
	float ratio = 1.0;

	bool running = true;

	//mouse input
	int mouse_button;
	int mouse_x = 0;
	int mouse_y = 0;
	float scale = 0.3;
	float x_angle = 0.0;
	float y_angle = 0.0;
	float angle = 0.0f;

	//keyboard
	Keyboard *keyboard;

	//nurbs data
	int N = 20;
	float verticies[20][3];
	float Geometry[4][3] = {
		{ 5000, 200, 2500 },  //  Point 1
		{ -5000,  200, -1500 },	  //	Point2
		{ 5000,  200, 2000 },	  //	Tangent1
		{ -5000,   200, -1400 }		//	Tangent2
	};

	//game objects
	Lane* lane;
	Wall* wall;
	Skybox* skybox;
	FlatPlain *flatPlain;
	Box *box;
	Player *p1;
	ParticleSystem* ps;

	//variables for camera and player position
	float box_pos_x = 0;
	float box_pos_y = -800;
	float box_pos_z = 200;

	float camera_pos_x = box_pos_x + 15; //orig 0
	float camera_pos_y = box_pos_y + 660; //original pos_y = -135.0f
	float camera_pos_z = box_pos_z + 50; //orig 0

	float camera_look_x = box_pos_x + 15; //orig 0
	float camera_look_y = box_pos_y + 640; //orig -200
	float camera_look_z = box_pos_z - 50; //orig -1

	float oldPosX = 0;
	float oldPosY = 0;
	float oldPosZ = 0;
	float oldLookX = 0;
	float oldLookY = 0;
	float oldLookZ = 0;
};