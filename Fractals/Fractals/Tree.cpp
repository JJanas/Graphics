//Erik Isaacson, Jacob Janas
#include <vector>
#include "Imath/imathvec.h"
#include "GL/glut.h"
#include <string>
#include <fstream>
#include <math.h>
#include <limits.h>           
#include <float.h>          
#include <time.h>
#include "common.h"

#define random_number() (((float) rand())/((float) RAND_MAX))
#define MAXLEVEL 8
int Level = 4;
long TreeSeed;
int TREE = 0, STEM = 1, TREE_MAT = 4, FULLTREE = 10;

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

// reshape
void reshape(int w, int h) {
	width = w;
	height = h;
	if (h == 0) h = 1;
	ratio = 1.0f * w / h;
}

// init
void init() {
	// init tree
	CreateTreeLists();
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	ratio = (double)width / (double)height;
	// mesh
	mesh1 = createPlane(2000, 2000, 200);
	calculateNormalPerFace(mesh1);
	calculateNormalPerVertex(mesh1);
	display1 = meshToDisplayList(mesh1, 1, 0);
	// light
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45, ratio, 1, 1000);
	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// lookAt
	gluLookAt(0.0f, 70.0f, 320.0, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	// camera
	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	//plane
	glPushMatrix();
	glTranslatef(-900, 0, -900);
	glCallList(display1);
	glPopMatrix();
	// tree fractal
	glPushMatrix();
	glTranslatef(300, 100, 300);
	glScalef(100, 100, 100);
	glCallList(FULLTREE);
	glPopMatrix();
	// end
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// texto
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, height - 13.0f, 0.0f, "Use [Mouse Left Key] to rotate");
	renderBitmapString(0.0, height - 26.0f, 0.0f, "Use [Mouse Right Key] to scale");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutSwapBuffers();
}

// main
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Fractals");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	init();
	glutMainLoop();
}