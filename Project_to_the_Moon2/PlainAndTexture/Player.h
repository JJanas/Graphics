#pragma once
#include "Globals.h"

class Player {

public:
	Player();
	~Player();
	Mesh* getMesh();
	GLuint getDisplayList();
	GLuint getBoundingBox();
	void calculateNormalPerFace();
	void calculateNormalPerVertex();
	void meshToDisplayList();
	Vec3<GLfloat> boundingMinPoint;
	Vec3<GLfloat> boundingMaxPoint;

private:
	Mesh* mesh;
	GLuint display;
	GLuint texture;
	GLuint boundingBox;
};