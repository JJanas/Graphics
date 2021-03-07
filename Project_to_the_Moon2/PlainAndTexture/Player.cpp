#include "Player.h"

Player::Player() {

	//get the mesh from the obj file
	this->mesh = loadFile("./object/Astro.txt");
	calculateNormalPerFace();
	calculateNormalPerVertex();
	meshToDisplayList();
	calculateAABB(this->mesh->dot_vertex, this->boundingMaxPoint, this->boundingMinPoint);
	aabbToDisplayList(boundingBox, this->boundingMaxPoint, this->boundingMinPoint);
}

//default destructor
Player::~Player() {
	//basic
}

Mesh* Player::getMesh() {
	return this->mesh;
}

GLuint Player::getDisplayList() {
	return this->display;
}

GLuint Player::getBoundingBox() {
	return this->boundingBox;
}

//normal per face
void Player::calculateNormalPerFace() {
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
void Player::calculateNormalPerVertex() {
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
void Player::meshToDisplayList() {
	this->display = glGenLists(1);
	glNewList(this->display, GL_COMPILE);
	//if (id != 3) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	//}
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < this->mesh->face_index_vertex.size(); i++) {
		// PER VERTEX NORMALS
		if ((!this->mesh->dot_normalPerVertex.empty() && !this->mesh->face_index_normalPerVertex.empty())) {
			glNormal3fv(&this->mesh->dot_normalPerVertex[this->mesh->face_index_normalPerVertex[i]].x);
		}
		// TEXTURES
		//if (!this->mesh->dot_texture.empty() && !this->mesh->face_index_texture.empty()) {
			//glTexCoord2fv(&this->mesh->dot_texture[this->mesh->face_index_texture[i]].x);
		//}
		// COLOR
		//Vec3f offset = (this->mesh->dot_vertex[this->mesh->face_index_vertex[i]]); //uncomment these for normal lava effect
		// VERTEX
		//glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z)); //this one too
		glVertex3fv(&this->mesh->dot_vertex[this->mesh->face_index_vertex[i]].x);
	}
	glEnd();
	//if (id != 3) {
	glDisable(GL_TEXTURE_2D);
	//}
	glEndList();
}