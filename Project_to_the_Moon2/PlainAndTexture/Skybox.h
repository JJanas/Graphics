#pragma once
#include "Globals.h"

class Skybox {

public:
	Skybox();
	~Skybox();
	Mesh* getMesh();
	GLuint getDisplayList();
	Mesh* createCube();
	void calculateNormalPerFace();
	void calculateNormalPerVertex();
	void skyboxToDisplayList();
	void bmpTexture(UINT textureArray[], const char *file, int n);

private:
	Mesh* mesh;
	GLuint display;
	GLuint textureArray[6];
};