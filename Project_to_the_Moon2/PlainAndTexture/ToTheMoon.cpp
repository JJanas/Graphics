#include "ToTheMoon.h"
#include "timer.h"
#include <vector>
#include "Imath/imathvec.h"
#include "GL/glut.h"
#include <string>
#include <fstream>
#include <math.h>
#include <limits.h>           
#include <float.h>          
#include <time.h>

#define random_number() (((float) rand())/((float) RAND_MAX))
#define MAXLEVEL 8
int Level = 4;
long TreeSeed;
int TREE = 0, STEM = 1, TREE_MAT = 4, FULLTREE = 10;

const int V_size = 4;
const int U_size = 4;
const int ORDER = 4;
// Knot sequences for cubic bezier surface and trims 
GLfloat sknots[V_size + ORDER] = { 0, 0, 0, 0, 3, 3, 3, 3 };
GLfloat tknots[U_size + ORDER] = { 0, 0, 0, 0, 3, 3, 3, 3 };

// Control points for the flag. The Z values are modified to make it wave
GLfloat ctlpoints[V_size][U_size][3] = {
	{ { 0, 300, 0 },{ 100, 300, 0 },{ 200, 300, 0 },{ 300, 300, 0 } },
	{ { 0, 200, 0 },{ 100, 200, 0 },{ 200, 200, 0 },{ 300, 200, 0 } },
	{ { 0, 100, 0 },{ 100, 100, 0 },{ 200, 100, 0 },{ 300, 100, 0 } },
	{ { 0, 0, 0 },{ 100, 0, 0 },{ 200, 0, 0 },{ 300, 0, 0 } }
};

GLUnurbsObj *nurbsflag;


// draw_nurb
void draw_nurb() {
	static GLfloat angle = 0.0;
	int i, j;
	// wave the flag by rotating Z coords though a sine wave
	for (i = 1; i<4; i++)
		for (j = 0; j<4; j++)
			ctlpoints[i][j][2] = sin((GLfloat)i + angle);
	angle += 0.1;
	glPushMatrix();
	glTranslatef(-300, 0, 200);
	glScalef(1.5, 1.0, 1.0);
	glRotatef(90, 0, 0, 1);
	gluBeginSurface(nurbsflag);
	gluNurbsSurface(nurbsflag, (V_size + ORDER), sknots, (U_size + ORDER), tknots,
		3 * U_size, 3,
		&ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluNurbsSurface(nurbsflag, (V_size + ORDER), sknots, (U_size + ORDER), tknots,
		3 * U_size, 3,
		&ctlpoints[0][0][0], 4, 4, GL_MAP2_NORMAL);
	gluEndSurface(nurbsflag);
	glPopMatrix();
}

// timer
void timer(int t) {
	draw_nurb();
	glutPostRedisplay();
	glutTimerFunc(1000.0 / 30.0, timer, 0);
}

// create a tree as fractal
void FractalTree(int level) {
	long savedseed;
	if (level == Level) {
		glPushMatrix();
		glRotatef(random_number() * 180, 0, 1, 0);
		glCallList(STEM);
		glPopMatrix();
	}
	else {
		glCallList(STEM);
		glPushMatrix();
		glRotatef(random_number() * 180, 0, 1, 0);
		glTranslatef(0, 1, 0);
		glScalef(0.7, 0.7, 0.7);
		glPushMatrix();
		glRotatef(110 + random_number() * 40, 0, 1, 0);
		glRotatef(30 + random_number() * 20, 0, 0, 1);
		FractalTree(level + 1);
		glPopMatrix();

		glPushMatrix();
		glRotatef(-130 + random_number() * 40, 0, 1, 0);
		glRotatef(30 + random_number() * 20, 0, 0, 1);
		FractalTree(level + 1);
		glPopMatrix();

		glPushMatrix();
		glRotatef(-20 + random_number() * 40, 0, 1, 0);
		glRotatef(30 + random_number() * 20, 0, 0, 1);
		FractalTree(level + 1);
		glPopMatrix();
		glPopMatrix();
	}
}

// Create display lists for a leaf, a set of leaves, and a stem
void CreateTreeLists(void) {
	// materials
	GLfloat tree_ambuse[] = { 0.4, 0.25, 0.1, 1.0 };
	GLfloat tree_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat tree_shininess[] = { 0 };
	// tree material
	glNewList(TREE_MAT, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tree_ambuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tree_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, tree_shininess);
	glEndList();
	// steam
	glNewList(STEM, GL_COMPILE);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glBegin(GL_QUADS);
	//Top Face z = 1
	glVertex3f(0.08f, -0.08f, 1.0f);
	glVertex3f(-0.08f, -0.08f, 1.0f);
	glVertex3f(-0.08f, 0.08f, 1.0f);
	glVertex3f(0.08f, 0.08f, 1.0f);
	//Bottom Face z = 0
	glVertex3f(0.1f, 0.1f, 0.0f);
	glVertex3f(-0.1f, 0.1f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	//Front Face y = 0.08 - 0.1
	glVertex3f(0.08f, 0.08f, 1.0f);
	glVertex3f(-0.08f, 0.08f, 1.0f);
	glVertex3f(-0.1f, 0.1f, 0.0f);
	glVertex3f(0.1f, 0.1f, 0.0f);
	//Back Face y = -0.1 - -0.08
	glVertex3f(0.1f, -0.1f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(-0.08f, -0.08f, 1.0f);
	glVertex3f(0.08f, -0.08f, 1.0f);
	//Left Face x = -0.1 - -0.08
	glVertex3f(-0.08f, 0.08f, 1.0f);
	glVertex3f(-0.08f, -0.08f, 1.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(-0.1f, 0.1f, 0.0f);
	//Right Face x = 0.08 - 0.1
	glVertex3f(0.08f, -0.08f, 1.0f);
	glVertex3f(0.08f, 0.08f, 1.0f);
	glVertex3f(0.1f, 0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glEnd();
	glPopMatrix();
	glEndList();
	//
	glNewList(FULLTREE, GL_COMPILE);
	glPushMatrix();
	glPushAttrib(GL_LIGHTING_BIT);
	glCallList(TREE_MAT);
	glTranslatef(0, -1, 0);
	FractalTree(0);
	glPopAttrib();
	glPopMatrix();
	glEndList();
}

ToTheMoon::ToTheMoon() {
	//basic constructor
}

ToTheMoon::~ToTheMoon() {
	//generic destructor
}

//init function
void ToTheMoon::init() {

	//setup graphics enviornment and objects within the world
	//this->createMenus();
	CreateTreeLists();
	glEnable(GL_AUTO_NORMAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	this->ratio = (double)width / (double)height;

	//initialize frame timer
	init_frame_timer();

	//create game objects
	this->lane = new Lane();
	//this->wall = new Wall();
	this->skybox = new Skybox();
	this->flatPlain = new FlatPlain(1000, 1000);
	this->box = new Box(100, 100, 100);
	this->p1 = new Player();
	this->ps = new ParticleSystem();
	this->keyboard = new Keyboard();
	float base_camera_look = this->camera_look_y;
	float base_camera_pos = this->camera_pos_y;
	float base_box_pos = this->box_pos_y;


	//light
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //0.6, 0.6, 0.6, 0.5
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; //0.3, 0.3, 0.3, 0.3
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //0.5, 0.5, 0.5, 0.3
	GLfloat light_position[] = { 0.0, 200.0, 1.0, 0.0 }; //0.0, 0.0, 3500.0, 0.0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//clear stencil buffer
	glClearStencil(0);

	//fog settings
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.25);
	glFogf(GL_FOG_START, 3000.0);
	glFogf(GL_FOG_END, 6000.0);

	nurbsflag = gluNewNurbsRenderer();
	gluNurbsProperty(nurbsflag, GLU_SAMPLING_TOLERANCE, 100.0);
	gluNurbsProperty(nurbsflag, GLU_DISPLAY_MODE, GLU_FILL); //GLU_OUTLINE_POLYGON
}

void ToTheMoon::render() {

	//draw graphics to the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_CULL_FACE);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glViewport(0, 0, this->width, this->height);
	gluPerspective(45, this->ratio, 1, 10000);

	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// lookAt
	gluLookAt(this->camera_pos_x, this->camera_pos_y, this->camera_pos_z,
		this->camera_look_x, this->camera_look_y, this->camera_look_z,
		0.0f, 1.0f, 0.0f);

	// camera
	glScalef(this->scale, this->scale, this->scale);
	glRotatef(this->x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(this->y_angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);

	//create the stencil
	/*glEnable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //Disable writing colors in frame buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF); //Place a 1 where rendered
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); 	//Replace where rendered

		// mirror for the stencil
		glPushMatrix();
		glTranslatef(-400, 0, 0);
		glRotatef(90, 1, 0, 0);
		glCallList(mirror->getDisplayList());
		glPopMatrix();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Reenable color
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Keep the pixel

	//draw reflected box
	glPushMatrix();
	glTranslatef(box_pos_x * -1, -800, box_pos_z * -1);
	glScalef(1, 1, -1);
	glCallList(box->getDisplayList());
	glPopMatrix();

	//diable stencil
	glDisable(GL_STENCIL_TEST); */

	//plain
	//glPushMatrix();
	//glTranslatef(-5000, -800, -5000);
	//glCallList(lane->getDisplayList());
	//glPopMatrix();

	// tree fractal
	glPushMatrix();
	glTranslatef(20, -700, 20);
	glScalef(100, 100, 100);
	glCallList(FULLTREE);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -700, 800);
	glScalef(100, 100, 100);
	glCallList(FULLTREE);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-200, -700, 700);
	glScalef(100, 100, 100);
	glCallList(FULLTREE);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-100, -700, 900);
	glScalef(100, 100, 100);
	glCallList(FULLTREE);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(400, -700, 700);
	glScalef(100, 100, 100);
	glCallList(FULLTREE);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-300, -700, 400);
	glScalef(100, 100, 100);
	glCallList(FULLTREE);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(100, -750, 100);
	glCallList(this->box->getDisplayList());
	glPopMatrix();

	// Displaying Fog
	/*********************************************************************/
	if (this->showFog == false) {
		glDisable(GL_FOG);
	}
	else if (this->showFog) {
		glEnable(GL_FOG);
	}
	/*********************************************************************/

	// Skybox
	/*********************************************************************/
	if (showSkyBox) {
		glPushMatrix();
		glTranslatef(-10000, -3000, -10000); //-2500, -1000, -2500
		glCallList(this->skybox->getDisplayList());
		glPopMatrix();
	}
	/*********************************************************************/

	// Displaying Flat Plain
	/*********************************************************************/
	glPushMatrix();
	glTranslatef(-5000, -800, -5000);
	glCallList(this->lane->getDisplayList());
	glPopMatrix();
	//Walls
	/*
	glPushMatrix();
	glTranslatef(0, -600, 0);
	glCallList(this->wall->getDisplayList());
	glPopMatrix();
	*/
	/*********************************************************************/

	//Shooting Star(Hermite)
	/*********************************************************************/
	glBegin(GL_LINE_STRIP);
	// use the parametric time value 0 to 1
	for (int i = 0; i != N; ++i) {
		float t = (float)i / (N - 1);
		// calculate blending functions
		float b0 = 2 * t*t*t - 3 * t*t + 1;
		float b1 = -2 * t*t*t + 3 * t*t;
		float b2 = t * t*t - 2 * t*t + t;
		float b3 = t * t*t - t * t;
		// calculate the x, y and z of the curve point
		float x = b0 * Geometry[0][0] + b1 * Geometry[1][0] + b2 * Geometry[2][0] + b3 * Geometry[3][0];
		float y = b0 * Geometry[0][1] + b1 * Geometry[1][1] + b2 * Geometry[2][1] + b3 * Geometry[3][1];
		float z = b0 * Geometry[0][2] + b1 * Geometry[1][2] + b2 * Geometry[2][2] + b3 * Geometry[3][2];
		verticies[i][0] = x;
		verticies[i][1] = y;
		verticies[i][2] = z;
		// specify the point
		glVertex3f(x, y, z);
	}
	glEnd();

	int point = 0;
	glPointSize(20.0);
	glColor3f(1, 1, 0);
	glPushMatrix();
	glBegin(GL_POINTS);
	if (point >= 1) {
		glTranslatef(verticies[point][0] - verticies[point - 1][0], verticies[point][1] - verticies[point - 1][1], verticies[point][2] - verticies[point - 1][2]);
		point++;
	}
	if (point == 19) {
		point = 0;
	}
	glVertex3f(verticies[0][0], verticies[0][1], verticies[0][2]);
	glEnd();
	glPopMatrix();

	/*********************************************************************/
	//Particle system attatched to the star
	/*********************************************************************/

	//createing particle system
	ps->add();
	float time = calculate_frame_time();
	ps->update(time);
	ps->remove();

	//drawing particle system
	glPushMatrix();
	drawParticles();
	glPopMatrix();

	/*********************************************************************/
	//enable blend and disable light
	/*glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.7, 0.0, 0.0, 0.3);
	glColor4f(1.0, 1.0, 1.0, 0.3);
	//draw the mirror
	glPushMatrix();
	glTranslatef(-400, 0, 0);
	glRotatef(90, 1, 0, 0);
	glCallList(mirror->getDisplayList());
	glPopMatrix();

	//enable lighting and disable blend
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);*/

	//lava
	/*
	glPushMatrix();
	glTranslatef(-10000, -800, -10000);
	glCallList(flatPlain->getDisplayList());
	glPopMatrix();*/

	//Create Game Objects
	/************************************************************************************/
	//box
	/*glPushMatrix();
	glTranslatef(0, -800, 200);
	glCallList(astroDisplayList);
	glPopMatrix();*/
	//box
	glPushMatrix();
	glTranslatef(this->box_pos_x, this->box_pos_y, this->box_pos_z);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(5.0, 5.0, 5.0);
	glCallList(this->p1->getDisplayList());
	glPopMatrix();
	/************************************************************************************/

	//Bounding Boxes - By default are enabled
	/************************************************************************************/
	if (showBoundingBox) {
		//draw the bounding box around the box
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(box_pos_x, box_pos_y, box_pos_z);
		glScalef(5.0, 5.0, 5.0);
		glCallList(p1->getBoundingBox());
		glEnable(GL_LIGHTING);
		glPopMatrix();

	}
	/************************************************************************************/

	/************************************************************************************/
	//drawFlag
	glPushMatrix();
	//material for flag
	GLfloat mat_diffuse[] = { 1.0f, 0.5f, 0.31f, 1. };
	GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1. };
	GLfloat mat_ambient[] = { 1.0f, 0.5f, 0.31f, 1. };

	//creating materials
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0);
	draw_nurb();
	glPopMatrix();
	/************************************************************************************/

	// end - of all drawing
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// texto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, this->width, 0, this->height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, this->height - 13.0f, 0.0f, "Use w, a, s, d, to move camera around.");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();



	glDisable(GL_CULL_FACE);
	

	glutSwapBuffers();
}

//function to handle the game logic
void ToTheMoon::logic() {

	bool box_y_changed = false;
	//handle input from the user
	if (this->keyboard->getKeys()[P1_FORWARD]) {
		this->camera_look_z += -10;
		this->camera_pos_z += -10;
		this->box_pos_z += -33.333333;
	}

	if (this->keyboard->getKeys()[P1_LEFT]) {
		this->camera_look_x += -10;
		this->camera_pos_x += -10;
		this->box_pos_x += -33.333333;
	}

	if (this->keyboard->getKeys()[P1_RIGHT]) {
		this->camera_look_x += 10;
		this->camera_pos_x += 10;
		this->box_pos_x += 33.333333;
	}

	if (this->keyboard->getKeys()[P1_BACK]) {
		this->camera_look_z += 10;
		this->camera_pos_z += 10;
		this->box_pos_z += 33.333333;
	}

	if (this->keyboard->getKeys()[P1_BOOST]) {
		this->camera_look_y += 10;
		this->camera_pos_y += 10;
		this->box_pos_y += 33.333333;
		box_y_changed = true;
	}
	if (box_y_changed) {
		this->camera_look_y += -10;
		this->camera_pos_y += -10;
		this->box_pos_y += -33.333333;
	}
	
}

void ToTheMoon::setEditorCamera() {
	//store old camera pos
	this->oldPosX = this->camera_pos_x;
	this->oldPosY = this->camera_pos_y;
	this->oldPosZ = this->camera_pos_z;
	this->oldLookX = this->camera_look_x;
	this->oldLookY = this->camera_look_y;
	this->oldLookZ = this->camera_look_z;
	//set editor camera pos & look
	this->camera_pos_x = 500;
	this->camera_pos_y = -100;
	this->camera_pos_z = 300;
	this->camera_look_x = 0;
	this->camera_look_y = -160;
	this->camera_look_z = 0;
}
//restore the player camera
void ToTheMoon::loadPlayerCamera() {
	this->scale = .3f;
	this->x_angle = 0;
	this->y_angle = 0;
	this->camera_pos_x = this->oldPosX;
	this->camera_pos_y = this->oldPosY;
	this->camera_pos_z = this->oldPosZ;
	this->camera_look_x = this->oldLookX;
	this->camera_look_y = this->oldLookY;
	this->camera_look_z = this->oldLookZ;
}

//Game loop to handle the logic and render
void ToTheMoon::gameLoop() {
	//check to see that the game is running and set frame rate
	int t1, t2;
	t1 = glutGet(GLUT_ELAPSED_TIME);
	if (this->running) {
		this->logic();
		this->render();
	}
	do {
		t2 = glutGet(GLUT_ELAPSED_TIME);
	} while (t2 - t1 < 1000 / FRAMERATE);
}

//read in mouse input
void ToTheMoon::mouse(int button, int state, int x, int y) {
	this->mouse_x = x;
	this->mouse_y = y;
	this->mouse_button = button;
}

//motion around scene
void ToTheMoon::motion(int x, int y) {
	if (mouse_button == GLUT_LEFT_BUTTON) {
		y_angle += (float(x - mouse_x) / width) * 360.0;
		x_angle += (float(y - mouse_y) / width) * 360.0;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON) {
		scale += (y - mouse_y) / 100.0;
		if (scale < 0.1) scale = 0.1;
		if (scale > 7)	scale = 7;
	}
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}

//rotate the camera
void ToTheMoon::rotate_point(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	// translate point back to origin:
	this->camera_look_x -= this->camera_pos_x;
	this->camera_look_z -= this->camera_pos_z;
	// rotate point
	float xnew = this->camera_look_x * c - this->camera_look_z * s;
	float znew = this->camera_look_x * s + this->camera_look_z * c;
	// translate point back:
	this->camera_look_x = xnew + this->camera_pos_x;
	this->camera_look_z = znew + this->camera_pos_z;
}

//reshape
void ToTheMoon::reshape(int w, int h) {
	this->width = w;
	this->height = h;
	if (h == 0) h = 1;
	this->ratio = 1.0f * w / h;
}

//return the keyboard input
Keyboard* ToTheMoon::getKeyboard() {
	return this->keyboard;
}

//write passed in text to screen
void ToTheMoon::renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

// draw particles
void ToTheMoon::drawParticles() {
	Particle* curr = ps->particle_head;
	// glPointSize(2);
	// glBegin(GL_POINTS);
	// while (curr) {
	//   glVertex3fv(curr->position);
	//	 curr = curr->next;
	// }
	// glEnd();
	while (curr) {
		glPushMatrix();
		//glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(100.0, 100.0, 100.0);
		glTranslatef(curr->position[0], curr->position[1], curr->position[2]);
		glScalef(0.001, 0.001, 0.001);
		glCallList(box->getDisplayList());
		glPopMatrix();
		curr = curr->next;
	}

}

/*
void ToTheMoon::drawNurb() {
	static GLfloat angle = 0.0;
	int i, j;
	// wave the flag by rotating Z coords though a sine wave
	for (i = 1; i < 4; i++)
		for (j = 0; j < 4; j++)
			ctlpoints[i][j][2] = sin((GLfloat)i + angle);
	angle += 0.1;
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0, 100.0, 0.0);
	//glScalef(12.5, 12.0, 12.0);
	glRotatef(90, 0., 0., 1.);
	gluBeginSurface(nurbsflag);
	gluNurbsSurface(nurbsflag, V_NUMKNOTS, sknots, U_NUMKNOTS, tknots,
		3 * U_NUMPOINTS, 3,
		&ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurbsflag);
	glPopMatrix();
}
*/