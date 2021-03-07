
#include "Box.h"

Box::Box(int w, int h, int d) {
	//create box
	//set variables first
	this->width = w;
	this->height = h;
	this->depth = d;
	bmpTexture(this->texture, "bmp/metal.bmp");
	this->mesh = createCube();
	calculateNormalPerFace();
	calculateNormalPerVertex();
	boxToDisplayList();
	calculateAABB(this->mesh->dot_vertex, this->boundingMaxPoint, this->boundingMinPoint);
	aabbToDisplayList();
}

Box::~Box() {
	//destruct
}

Mesh* Box::getMesh() {
	return this->mesh;
}

GLuint Box::getDisplayList() {
	return this->display;
}

GLuint Box::getBoundingBox() {
	return this->boundingBox;
}

void Box::bmpTexture(UINT texture, const char *file) {
	BITMAPINFO *bitmapInfo; // Bitmap information
	GLubyte    *bitmapBits; // Bitmap data
	if (!file) {
		cout << "texture file not found!" << endl;
		return;
	}
	bitmapBits = LoadDIBitmap(file, &bitmapInfo);
	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
																				 // glTexImage2D Whith size and minification
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmapInfo->bmiHeader.biWidth, bitmapInfo->bmiHeader.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmapBits);
}

Mesh* Box::createCube() {
	Mesh *mesh = new Mesh;
	// Vertexes
	//int BOX_WIDTH = 100;
	//int BOX_HEIGHT = 100;
	//int BOX_DEPTH = 100;

	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, this->height, this->depth)); //0
	mesh->dot_vertex.push_back(Vec3<GLfloat>(this->width, this->height, this->depth)); //1
	mesh->dot_vertex.push_back(Vec3<GLfloat>(this->width, 0.0, this->depth));//2
	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, this->depth));//3

	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, this->height, 0.0));//4
	mesh->dot_vertex.push_back(Vec3<GLfloat>(this->width, this->height, 0.0));//5
	mesh->dot_vertex.push_back(Vec3<GLfloat>(this->width, 0.0, 0.0));//6
	mesh->dot_vertex.push_back(Vec3<GLfloat>(0.0, 0.0, 0.0));//7

	// face 1
	mesh->face_index_vertex.push_back(1);//0
	mesh->face_index_vertex.push_back(3);//1
	mesh->face_index_vertex.push_back(0);//2
	mesh->face_index_vertex.push_back(1);//0
	mesh->face_index_vertex.push_back(2);//2
	mesh->face_index_vertex.push_back(3);//3
																			 // face 2 cieling
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(5);//1
	mesh->face_index_vertex.push_back(1);//5
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(4);//5
	mesh->face_index_vertex.push_back(5);//4
																			 // face 3
	mesh->face_index_vertex.push_back(5);//1
	mesh->face_index_vertex.push_back(2);//2
	mesh->face_index_vertex.push_back(1);//6
	mesh->face_index_vertex.push_back(5);//1
	mesh->face_index_vertex.push_back(6);//6
	mesh->face_index_vertex.push_back(2);//5
																			 // face 4 floor
	mesh->face_index_vertex.push_back(7);//2
	mesh->face_index_vertex.push_back(2);//3
	mesh->face_index_vertex.push_back(6);//7
	mesh->face_index_vertex.push_back(7);//2
	mesh->face_index_vertex.push_back(3);//7
	mesh->face_index_vertex.push_back(2);//6
																			 // face 5
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(7);//3
	mesh->face_index_vertex.push_back(4);//7
	mesh->face_index_vertex.push_back(0);//0
	mesh->face_index_vertex.push_back(3);//7
	mesh->face_index_vertex.push_back(7);//4
																			 // face 6
	mesh->face_index_vertex.push_back(4);//4 
	mesh->face_index_vertex.push_back(6);//5
	mesh->face_index_vertex.push_back(5);//6
	mesh->face_index_vertex.push_back(4);//4
	mesh->face_index_vertex.push_back(7);//6
	mesh->face_index_vertex.push_back(6);//7

																			 // texture
	mesh->dot_texture.push_back(Vec2<GLfloat>(0.0, 1.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(1.0, 1.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(1.0, 0.0));
	mesh->dot_texture.push_back(Vec2<GLfloat>(0.0, 0.0));

	for (int t = 0; t < 6; t++) {
		mesh->face_index_texture.push_back(0);//0
		mesh->face_index_texture.push_back(2);//1
		mesh->face_index_texture.push_back(1);//2
		mesh->face_index_texture.push_back(0);//0
		mesh->face_index_texture.push_back(3);//2
		mesh->face_index_texture.push_back(2);//3
	}

	return mesh;
}

//normal per face
void Box::calculateNormalPerFace() {
	Vec3<float> v1, v2, v3, v4, v5;
	for (int i = 0; i < this->mesh->face_index_vertex.size(); i += 3) {
		v1 = this->mesh->dot_vertex[this->mesh->face_index_vertex[i]];
		v2 = this->mesh->dot_vertex[this->mesh->face_index_vertex[i + 1]];
		v3 = this->mesh->dot_vertex[this->mesh->face_index_vertex[i + 2]];
		v4 = (v2 - v1);
		v5 = (v3 - v1);
		v4 = v4.cross(v5);
		v4.normalize();
		this->mesh->dot_normalPerFace.push_back(v4);
		int pos = this->mesh->dot_normalPerFace.size() - 1;
		// same normal for all vertex in this face
		this->mesh->face_index_normalPerFace.push_back(pos);
		this->mesh->face_index_normalPerFace.push_back(pos);
		this->mesh->face_index_normalPerFace.push_back(pos);
	}
}

// calculate normal per vertex
void Box::calculateNormalPerVertex() {
	this->mesh->dot_normalPerVertex.clear();
	this->mesh->face_index_normalPerVertex.clear();
	Vec3<float> suma; suma.x = 0; suma.y = 0; suma.z = 0;
	//initialize
	for (unsigned int val = 0; val < this->mesh->dot_vertex.size(); val++) {
		this->mesh->dot_normalPerVertex.push_back(suma);
	}
	// calculate sum for vertex
	for (long pos = 0; pos < this->mesh->face_index_vertex.size(); pos++) {
		this->mesh->dot_normalPerVertex[this->mesh->face_index_vertex[pos]] +=
			this->mesh->dot_normalPerFace[this->mesh->face_index_normalPerFace[pos]];
	}
	// normalize for vertex 
	for (unsigned int val = 0; val < this->mesh->dot_normalPerVertex.size(); val++) {
		this->mesh->dot_normalPerVertex[val] = this->mesh->dot_normalPerVertex[val].normalize();
	}
	//normalVertexIndex is the same that vertexIndex
	for (unsigned int pos = 0; pos < this->mesh->face_index_vertex.size(); pos++) {
		this->mesh->face_index_normalPerVertex.push_back(this->mesh->face_index_vertex[pos]);
	}
}

// draw
void Box::boxToDisplayList() {
	this->display = glGenLists(1);
	glNewList(this->display, GL_COMPILE);

	int index = 0;
	//if (id != 3) {
	for (int x = 0; x < 6; x++) {
		//glEnable(GL_TEXTURE_2D);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//glBindTexture(GL_TEXTURE_2D, texture);
		//}
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 6; i++) {
			// PER VERTEX NORMALS
			if ((!this->mesh->dot_normalPerVertex.empty() && !this->mesh->face_index_normalPerVertex.empty())) {
				glNormal3fv(&this->mesh->dot_normalPerVertex[this->mesh->face_index_normalPerVertex[index + i]].x);
			}
			// TEXTURES
			//if (!this->mesh->dot_texture.empty() && !this->mesh->face_index_texture.empty()) {
				//glTexCoord2fv(&this->mesh->dot_texture[this->mesh->face_index_texture[index + i]].x);
			//}
			// COLOR
			//Vec3f offset = (this->mesh->dot_vertex[this->mesh->face_index_vertex[index + i]]);
			// VERTEX
			//glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3fv(&this->mesh->dot_vertex[this->mesh->face_index_vertex[index + i]].x);
		}
		glEnd();
		//if (id != 3) {
		index = index + 6;
		//}
	}
	//glDisable(GL_TEXTURE_2D);

	glEndList();
}

//create the bounding box
void Box::aabbToDisplayList() {
	this->boundingBox = glGenLists(1);
	glNewList(this->boundingBox, GL_COMPILE);

	//using color white and draw the lines of the bounding box
	//bottom square
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(this->boundingMinPoint.x, this->boundingMinPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMinPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMinPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMinPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMinPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMinPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMinPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMinPoint.y, this->boundingMaxPoint.z);

	//start top square

	glVertex3f(this->boundingMinPoint.x, this->boundingMaxPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMaxPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMaxPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMaxPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMaxPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMaxPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMaxPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMaxPoint.y, this->boundingMaxPoint.z);

	//connect squares

	glVertex3f(this->boundingMinPoint.x, this->boundingMinPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMaxPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMinPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMaxPoint.y, this->boundingMaxPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMinPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMaxPoint.x, this->boundingMaxPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMinPoint.y, this->boundingMinPoint.z);
	glVertex3f(this->boundingMinPoint.x, this->boundingMaxPoint.y, this->boundingMinPoint.z);
	glEnd();

	//end list
	glEndList();
}
