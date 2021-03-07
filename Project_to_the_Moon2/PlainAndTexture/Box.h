#pragma once
#include "Globals.h"

class Box {

public:
	Box(int w, int h, int d);
	~Box();
	Mesh* getMesh();
	GLuint getDisplayList();
	GLuint getBoundingBox();
	Mesh* createCube();
	void calculateNormalPerFace();
	void calculateNormalPerVertex();
	void boxToDisplayList();
	void aabbToDisplayList();
	void bmpTexture(UINT texture, const char *file);
	Vec3<GLfloat> boundingMinPoint;
	Vec3<GLfloat> boundingMaxPoint;

private:
	Mesh* mesh;
	GLuint display;
	GLuint texture;
	GLuint boundingBox;
	int width, height, depth;
};