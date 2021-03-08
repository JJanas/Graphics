#pragma once
#pragma warning(disable : 4996)

//File to store all the include libs as well as common used data structures
#include <vector>
#include <ImathVec.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <wingdi.h>
#include "noise.h"

#define PI 3.1415926
#define MIN2(a, b) (((a) < (b)) ? (a) : (b))
#define MAX2(a, b) (((a) > (b)) ? (a) : (b))

using namespace std;
using namespace Imath;

typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;

// mesh data structure
struct Mesh {
	// vertex
	vector<Vec3f> dot_vertex;
	vector<Vec3f> dot_normalPerFace;
	vector<Vec3f> dot_normalPerVertex;
	vector<Vec2f> dot_texture;
	// faces
	vector<int> face_index_vertex;
	vector<int> face_index_normalPerFace;
	vector<int> face_index_normalPerVertex;
	vector<int> face_index_texture;
};

// t_scale
inline float t_scale(float x) {
	// assuming noise report values -2.xxx to 2.xxx
	return (x + 2) / 4;
}

// marbleMap
inline Vec3f marbleMap(float a) {
	Vec3f black = Vec3f(0, 0, 0);
	Vec3f color = Vec3f(1, 1, 1);
	return ((1 - a) * color + a * black);
}

// skyMap
inline Vec3f skyMap(float a) {
	Vec3f white = Vec3f(1, 1, 1);
	Vec3f color = Vec3f(1, 0, 0);
	return ((1 - a) * white + a * color);
}

//lavaMap
inline Vec3f lava(float a) {
	Vec3f red = Vec3f(0, 0, 1);
	Vec3f color = Vec3f(0, 1, .9);
	return ((1 - a) * red + a * color);
}

// Load a DIB/BMP file from disk.
inline GLubyte *LoadDIBitmap(const char *filename, BITMAPINFO **info) {
	FILE *fp;      // open file pointer
	GLubyte * bits; // bitmap pixel bits
	int bitsize;   // Size of bitmap
	int infosize;  // Size of header information
	BITMAPFILEHEADER header; // File header
													 // try opening the file; use "rb" mode to read this *binary* file.
	if ((fp = fopen(filename, "rb")) == NULL)
		return (NULL);
	// read the file header and any following bitmap information.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		// Couldn't read the file header - return NULL.
		fclose(fp);
		return (NULL);
	}
	// Check for BM reversed.
	if (header.bfType != 'MB') {
		// not a bitmap file - return NULL.
		fclose(fp);
		return (NULL);
	}
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		// couldn't allocate memory for bitmap info - return NULL.
		fclose(fp);
		return (NULL);
	}
	if (fread(*info, 1, infosize, fp) < infosize) {
		// Couldn't read the bitmap header - return NULL.
		free(*info);
		fclose(fp);
		return (NULL);
	}
	// Now that we have all the header info read in, allocate memory for the bitmap and read *it* in.
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount + 7) / 8 * abs((*info)->bmiHeader.biHeight);
	if ((bits = (GLubyte *)malloc(bitsize)) == NULL) {
		// Couldn't allocate memory - return NULL!
		free(*info);
		fclose(fp);
		return (NULL);
	}
	if (fread(bits, 1, bitsize, fp) < bitsize) {
		// couldn't read bitmap - free memory and return NULL!
		free(*info);
		free(bits);
		fclose(fp);
		return (NULL);
	}
	// OK, everything went fine - return the allocated bitmap.
	fclose(fp);
	return (bits);
}

// OBJ file - str to int
inline int StrToInt(const string &str) {
	int i;
	if (sscanf_s(str.c_str(), "%i", &i) == 1) return i;
	else return 0;
}

// OBJ file - split string
inline vector<string> split_string(const string& str, const string& split_str) {
	vector<string> stlv_string;
	string part_string("");
	string::size_type i;
	i = 0;
	while (i < str.size()) {
		if (split_str.find(str[i]) != string::npos) {
			stlv_string.push_back(part_string);
			part_string = "";
			while (split_str.find(str[i]) != string::npos) {
				++i;
			}
		}
		else {
			part_string += str[i];
			++i;
		}
	}
	if (!part_string.empty())
		stlv_string.push_back(part_string);
	return stlv_string;
}

// OBJ file - load file
inline Mesh* loadFile(const char* file) {
	Mesh *m = new Mesh;
	m->dot_vertex.clear();
	m->face_index_vertex.clear();
	ifstream infile(file);
	if (infile.fail()) {
		cout << "Error opening file " << file;
		return NULL;
	}
	char current_line[1024];
	while (!infile.eof()) {
		infile.getline(current_line, 1024);
		switch (current_line[0]) {
		case'v':
			float x, y, z;
			switch (current_line[1]) {
			case 'n':
				sscanf_s(current_line, "vn %f %f %f", &x, &y, &z);
				m->dot_normalPerFace.push_back(Vec3f(x, y, z));
				m->dot_normalPerVertex.push_back(Vec3f(x, y, z));
				break;
			case 't':
				sscanf_s(current_line, "vt %f %f", &x, &y);
				m->dot_texture.push_back(Vec2f(x, y));
				break;
			default:
				sscanf_s(current_line, "v %f %f %f", &x, &y, &z);
				m->dot_vertex.push_back(Vec3f(x, y, z));
				break;
			}
			break;
		case 'f': {
			vector<string> faces = split_string(current_line, " \t.\r\n");
			int vnt[3][3] = { { -1, -1, -1 },{ -1, -1, -1 },{ -1, -1, -1 } };
			string::size_type begin, end;
			for (int i = 0; i < 2; i++) {
				begin = 0;
				int j = 0;
				do {
					end = faces[i + 1].find_first_of("/", begin);
					if (begin < end) {
						vnt[i][j] = StrToInt(faces[i + 1].substr(begin, end - begin)) - 1;
					}
					else {
						vnt[i][j] = -1;
					}
					begin = end + 1;
					j++;
				} while (end != string::npos);
			}
			for (unsigned int i = 3; i < faces.size(); i++) {
				begin = 0;
				int j = 0;
				do {
					end = faces[i].find_first_of("/", begin);
					if (begin < end) {
						vnt[2][j] = StrToInt(faces[i].substr(begin, end - begin)) - 1;
					}
					else {
						vnt[2][j] = -1;
					}
					begin = end + 1;
					j++;
				} while (end != string::npos);
				for (int j = 0; j < 3; j++) {
					m->face_index_vertex.push_back(vnt[j][0]);
					if (vnt[j][1] != -1) m->face_index_texture.push_back(vnt[j][1]);
					if (vnt[j][2] != -1) m->face_index_normalPerFace.push_back(vnt[j][2]);
					if (vnt[j][2] != -1) m->face_index_normalPerVertex.push_back(vnt[j][2]);
				}
				memcpy(&vnt[1], &vnt[2], sizeof(int) * 3);
			}
		}
				  break;
		default:
			break;
		}
	}
	return m;
}

//load bmp file
inline void loadBMP_custom(GLuint texture, const char * imagepath) {

	printf("Reading image %s\n", imagepath);
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
		getchar();
		return;
	}
	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    fclose(file); return; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    fclose(file); return; }
	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	// Create one OpenGL texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // must set to 1 for compact data
										   // glTexImage2D Whith size and minification
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	delete[] data;

	// Poor filtering, or ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	// ... nice trilinear filtering ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

}

inline void calculateAABB(vector<Vec3f> vec, Vec3<GLfloat> &boundMaxP, Vec3<GLfloat> &boundMinP) {
	
	//check to see if the vector is empty
	if (vec.empty()) return;

	boundMaxP = vec[0];
	boundMinP = vec[0];

	//iterate through the vector of dots to find the largest and smallest
	//per x, y, and z

	for (unsigned int i = 1; i < vec.size(); i++) {
		boundMinP.x = MIN2(boundMinP.x, vec[i].x);
		boundMinP.y = MIN2(boundMinP.y, vec[i].y);
		boundMinP.z = MIN2(boundMinP.z, vec[i].z);
		boundMaxP.x = MAX2(boundMaxP.x, vec[i].x);
		boundMaxP.y = MAX2(boundMaxP.y, vec[i].y);
		boundMaxP.z = MAX2(boundMaxP.z, vec[i].z);
	}

	boundMinP.x = boundMinP.x - 10;
	boundMinP.y = boundMinP.y - 10;
	boundMinP.z = boundMinP.z - 10;
	boundMaxP.x = boundMaxP.x + 10;
	boundMaxP.y = boundMaxP.y + 10;
	boundMaxP.z = boundMaxP.z + 10;
}
//create the bounding box
inline void aabbToDisplayList(GLuint &boundingBox, Vec3<GLfloat> boundingMaxPoint, Vec3<GLfloat> boundingMinPoint) {
	boundingBox = glGenLists(1);
	glNewList(boundingBox, GL_COMPILE);

	//using color white and draw the lines of the bounding box
	//bottom square
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(boundingMinPoint.x, boundingMinPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMinPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMinPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMinPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMinPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMinPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMinPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMinPoint.y, boundingMaxPoint.z);

	//start top square

	glVertex3f(boundingMinPoint.x, boundingMaxPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMaxPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMaxPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMaxPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMaxPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMaxPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMaxPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMaxPoint.y, boundingMaxPoint.z);

	//connect squares

	glVertex3f(boundingMinPoint.x, boundingMinPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMaxPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMinPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMaxPoint.y, boundingMaxPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMinPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMaxPoint.x, boundingMaxPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMinPoint.y, boundingMinPoint.z);
	glVertex3f(boundingMinPoint.x, boundingMaxPoint.y, boundingMinPoint.z);
	glEnd();

	//end list
	glEndList();
}