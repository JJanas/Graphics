/*
Lecture 25
http://slides.com/javiergs/ser332-l25
javiergs@asu.edu

Code reused from github above

Authors: Elizabeth Layman
		 Jacob Janas
		 Erik Isaacson

Date	: 8/30/2018
*/

#pragma warning(disable : 4996)


#include "Globals.h"
#include "Lane.h"
#include "Skybox.h"
#include "FlatPlain.h"
#include "Box.h"
#include "Player.h"
#include "particles.h"
#include "timer.h"

int width = 1200;
int height = 600;
float ratio = 1.0;

int N = 20;
float verticies[20][3];
float Geometry[4][3] = {
	{ 5000, 200, 2500 },  //  Point 1
	{ -5000,  200, -1500 },	  //	Point2
	{ 5000,  200, 2000 },	  //	Tangent1
	{ -5000,   200, -1400 }		//	Tangent2
};

//create variables to hold objects
Lane *lane;

/*
Creating a small enviornment with stencil and reflections
*/

Skybox *skybox;

FlatPlain *flatPlain; //for lava

//mirror
//FlatPlain *mirror;

//normal box
Box *box;

//astro model
Player *p1;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 0.3;
float x_angle = 0.0;
float y_angle = 0.0;

float box_pos_x = 0;
float box_pos_y = -800;
float box_pos_z = 200;

//variables to control camera position and looking at
float camera_pos_x = box_pos_x + 15; //orig 0
float camera_pos_y = box_pos_y + 660; //original pos_y = -135.0f
float camera_pos_z = box_pos_z + 50; //orig 0

float camera_look_x = box_pos_x + 15; //orig 0
float camera_look_y = box_pos_y + 640; //orig -200
float camera_look_z = box_pos_z - 50; //orig -1

float angle = 0.0f;

//create variables to control what features are displayed (default all are true)
bool showBoundingBox = true;
bool showFog = true;
bool showSkyBox = true;
//bool showFlatPlain = true;

//variable for timer
//float time = 0;

//create a menu listener and pass in menu option
void menuListener(int option) {
	//check passed in option
	switch (option) {
		case 1:
			if (showFog) {
				showFog = false;
			}
			else
				showFog = true;
			break;
		case 2:
			if (showBoundingBox) {
				showBoundingBox = false;
			}
			else
				showBoundingBox = true;
			break;
		case 3:
			if (showSkyBox) {
				showSkyBox = false;
			}
			else
				showSkyBox = true;
			break;
		/*case 4:
			if (showFlatPlain) {
				showFlatPlain = false;
			}
			else
				showFlatPlain = true;
			break;*/
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
	/*int flatPlainMenu = glutCreateMenu(menuListener);
	glutAddMenuEntry("Enable/Disable", 4);*/

	//create main menu
	int mainMenu = glutCreateMenu(menuListener);
	glutAddSubMenu("Fog", fogMenu);
	glutAddSubMenu("AABB", boundBoxMenu);
	glutAddSubMenu("Skybox", skyBoxMenu);
	//glutAddSubMenu("Flat Plane", flatPlainMenu);

	//attatch menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/**
 * meshToDisplayList
 *//*
GLuint astroToDisplayList(Mesh* m, int id) {
	GLuint listID = glGenLists(id);
	glNewList(listID, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < m->face_index_vertex.size(); i++) {
		if (!m->dot_normalPerVertex.empty() && !m->face_index_normalPerVertex.empty())
			glNormal3fv(&m->dot_normalPerVertex[m->face_index_normalPerVertex[i]].x);
		if (!m->dot_texture.empty() && !m->face_index_texture.empty())
			glTexCoord2fv(&m->dot_texture[m->face_index_texture[i]].x);
		// color
		Vec3f offset = (m->dot_vertex[m->face_index_vertex[i]]);
		glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
		glVertex3fv(&m->dot_vertex[m->face_index_vertex[i]].x);
	}
	glEnd();
	glEndList();
	return listID;
}*/

// init
void init() {
	createMenus();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	ratio = (double)width / (double)height;

	//initiate frame timer
	init_frame_timer();

	//create Lane object
	lane = new Lane();
	
	//create skybox object
	skybox = new Skybox();

	//mirror object
	//mirror = new FlatPlain(1000, 1000);

	//create flatPlain object
	flatPlain = new FlatPlain(1000, 1000);

	//create box for quiz 3
	box = new Box(100, 100, 100);

	p1 = new Player();

	//Mesh* mesh = loadFile("./obj/astro.obj");
	//if (mesh == NULL) exit(1);
	//astroDisplayList = astroToDisplayList(mesh, 1);


	//create cube object
	//cube = new Cube();

	// light
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //0.6, 0.6, 0.6, 0.5
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; //0.3, 0.3, 0.3, 0.3
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //0.5, 0.5, 0.5, 0.3
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 }; //0.0, 0.0, 3500.0, 0.0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glClearStencil(0);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.25);
	glFogf(GL_FOG_START, 3000.0);
	glFogf(GL_FOG_END, 6000.0);

}

// rotate what the user see
void rotate_point(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	// translate point back to origin:
	camera_look_x -= camera_pos_x;
	camera_look_z -= camera_pos_z;
	// rotate point
	float xnew = camera_look_x * c - camera_look_z * s;
	float znew = camera_look_x * s + camera_look_z * c;
	// translate point back:
	camera_look_x = xnew + camera_pos_x;
	camera_look_z = znew + camera_pos_z;
}

// reshape
void reshape(int w, int h) {
	width = w;
	height = h;
	if (h == 0) h = 1;
	ratio = 1.0f * w / h;
}

// mouse
/*void mouse(int button, int state, int x, int y) {
	mouse_x = x;
	mouse_y = y;
	mouse_button = button;
}*/

//keyboard
void keyboard(unsigned char key, int x, int y) {

	//move camera forward if w pressed
	if (key == 'a') {

		//change x pos of cam and box
		camera_look_x += -10;
		camera_pos_x += -10;
		box_pos_x += -33.333333;
		//rotation of the camera
		/*angle += -1.0f;
		rotate_point(-1);*/
	}

	//move camera back if s pressed

	else if (key == 'd') {

		camera_look_x += 10;
		camera_pos_x += 10;
		box_pos_x += 33.333333;
		//rotation of the camera
		/*angle += 1.0f;
		rotate_point(1);*/
	}

	else if (key == 'w') {
		//move camera pos
		//camera_pos_x += (10) * sin(angle);//*0.1;
		//camera_pos_z += (10) * -cos(angle);//*0.1;

		//move box pos z
		//box_pos_x += (25) * sin(angle);
		camera_look_z += -10;
		camera_pos_z += -10;
		box_pos_z += -33.333333;

		//change looking
		//camera_look_x += (10) * sin(angle);//*0.1;
		//camera_look_z += (10) * -cos(angle);//*0.1;
	}

	else if (key == 's') {
		//camera_z += 10;
		//camera_pos_x += (10) * sin(angle);//*0.1;
		//camera_pos_z += (-10) * -cos(angle);//*0.1;

		//move box pos z
		//box_pos_x += (-25) * sin(angle);
		camera_look_z += 10;
		camera_pos_z += 10;
		box_pos_z += 33.333333;

		//camera_viewing_y -= 10;
		//camera_look_x += (10) * sin(angle);//*0.1;
		//camera_look_z += (-10) * -cos(angle);//*0.1;
	}

	glutPostRedisplay();
}

// motion
/*void motion(int x, int y) {
	if (mouse_button == GLUT_LEFT_BUTTON) {
		y_angle += (float(x - mouse_x) / width) *360.0;
		x_angle += (float(y - mouse_y) / height)*360.0;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON) {
		scale += (y - mouse_y) / 100.0;
		if (scale < 0.1) scale = 0.1;
		if (scale > 7)	scale = 7;
	}
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}*/

// text
void renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

//draw particles
// draw particles
void drawParticles() {
	Particle* curr = ps.particle_head;
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

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45, ratio, 1, 10000);

	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// lookAt
	gluLookAt(camera_pos_x, camera_pos_y, camera_pos_z,
		camera_look_x, camera_look_y, camera_look_z,
		0.0f, 1.0f, 0.0f);

	// camera
	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
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

	// Displaying Fog
	/*********************************************************************/
	if (!showFog) {
		glDisable(GL_FOG);
	}
	else if (showFog) {
		glEnable(GL_FOG);
	}
	/*********************************************************************/

	// Skybox
	/*********************************************************************/
	if (showSkyBox) {
		glPushMatrix();
		glTranslatef(-10000, -3000, -10000); //-2500, -1000, -2500
		glCallList(skybox->getDisplayList());
		glPopMatrix();
	}
	/*********************************************************************/

	// Displaying Flat Plain
	/*********************************************************************/
	/*if (!showFlatPlain) {
		glPushMatrix();
		glTranslatef(-5000, -800, -5000);
		glCallList(lane->getPerlinDisplayList());
		glPopMatrix();
	}*/
	glPushMatrix();
	glTranslatef(-5000, -800, -5000);
	glCallList(lane->getDisplayList());
	glPopMatrix();
	/*********************************************************************/

	//Display Hill(NURBS)
	/*********************************************************************/
	// NURBS
	// V_size curves with U_size control points each
	// V_size + ORDER knots per curve and U_size + ORDER knots per inter-curve conection (controlpnt + 4)
	// V_size*3 and 3 offsets
	// cubic equations (4)

	const int V_size = 4;
	const int U_size = 4;
	const int ORDER = 4;
	GLfloat ctlpoints[U_size][V_size][3] = {
		{ { 70, -80, -20 } ,{ 70, -80, -10 },{ 70, -80, 10 },{ 70, -80, 20 } },
		{ { 90, -80, -20 } ,{ 90, -60, 0 },{ 90, -60, 0 },{ 90, -80, 20 } },
		{ { 90, -80, -20 } ,{ 90, -60, 0 },{ 90, -60, 0 },{ 90, -80, 20 } },
		{ { 110, -80, -20 } ,{ 110, -80, -10 },{ 110, -80, 10 },{ 110, -80, 20 } }
	};
	GLfloat vknots[V_size + ORDER] = { 0.0, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, 3.0 };
	GLfloat uknots[U_size + ORDER] = { 0.0, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, 3.0 };

	GLUnurbsObj *theNurb;
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	glPushMatrix();
	glScalef(10, 10, 10);
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb,
		U_size + ORDER, uknots,
		V_size + ORDER, vknots,
		V_size * 3,
		3,
		&ctlpoints[0][0][0],
		ORDER, ORDER,
		GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb);

	// control graph
	/*
	glDisable(GL_LIGHTING);
	glPointSize(1.0);
	glColor3f(0, 0, 1);
	for (int i = 0; i < U_size; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < V_size; j++) {
			glVertex3f(ctlpoints[i][j][0], ctlpoints[i][j][1], ctlpoints[i][j][2]);
		}
		glEnd();
	}
	*/
	// show control points
	/*
	glPointSize(5.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < U_size; i++) {
		for (int j = 0; j < V_size; j++) {
			glVertex3f(ctlpoints[i][j][0], ctlpoints[i][j][1], ctlpoints[i][j][2]);
		}
	}
	glEnd();
	*/
	glEnable(GL_LIGHTING);
	glPopMatrix();
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
		float b2 = t*t*t - 2 * t*t + t;
		float b3 = t*t*t - t * t;
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
			glTranslatef(verticies[point][0]-verticies[point - 1][0], verticies[point][1] - verticies[point - 1][1], verticies[point][2] - verticies[point - 1][2]);
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
	ps.add();
	float time = calculate_frame_time();
	ps.update(time);
	ps.remove();
	
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
	glTranslatef(box_pos_x, box_pos_y, box_pos_z);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(25.0, 25.0, 25.0);
	glCallList(p1->getDisplayList());
	glPopMatrix();
	/************************************************************************************/

	//Bounding Boxes - By default are enabled
	/************************************************************************************/
	/*if (showBoundingBox) {
		//draw the bounding box around the box
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(box_pos_x, box_pos_y, box_pos_z);
		glCallList(box->getBoundingBox());
		glEnable(GL_LIGHTING);
		glPopMatrix();

	}
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
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, height - 13.0f, 0.0f, "Use w, a, s, d, to move camera around.");
	renderBitmapString(0.0, height - 26.0f, 0.0f, "Use i, k, to move forward and back.");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();
}

// main
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Textures");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	init();

	glutMainLoop();
}