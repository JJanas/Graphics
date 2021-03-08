#pragma once
#include "Globals.h"

class FlatPlain {

public:
	FlatPlain(int w, int d);
	~FlatPlain();
	Mesh* getMesh();
	GLuint getDisplayList();
	GLuint getTexture();
	Mesh* createFlatPlane(int arena_cell);
	void calculateNormalPerFace();
	void calculateNormalPerVertex();
	void meshToDisplayList();
	void bmpTexture(UINT texture, const char *file);
	void codedTexture();

private:
	Mesh* mesh;
	GLuint display;
	GLuint texture;
	int width, depth;
};
