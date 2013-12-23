#pragma once

#include <cmath>
#include <string>
#include <vector>

#include "CGFtexture.h"
#include "CGFshader.h"
#include "CGFappearance.h"
#include "Animation.h"
#include "GL/glut.h"
#include "Socket.h"
#include <ctime>
#include <iostream>
using std::string;
using std::vector;

class Primitiva{
public:
	virtual void draw() = 0;
};

class Rectangle : public Primitiva
{
private:
	float x1, x2, y1, y2;
public:
	Rectangle(float x1, float x2, float y1, float y2);
	void draw();
};

class Triangle : public Primitiva
{
private:
	float x1, x2, x3, y1, y2, y3, z1, z2, z3;
	float normal_x, normal_y, normal_z;
	void calculateTexCoords();
	float lengthAB, lengthAC, texCoord_x, texCoord_y;
public:
	Triangle(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3);
	void draw();
};

class Cylinder : public Primitiva
{
private:
	float base, top, height;
	int slices, stacks;
public:
	Cylinder(float base, float top, float height, int slices, int stacks);
	void draw();
};

class Sphere : public Primitiva
{
private:
	float radius;
	int slices, stacks;
public:
	Sphere();
	Sphere(float radius, int slices, int stacks);
	void draw();
};

class Torus : public Primitiva
{
private:
	float inner, outer;
	int slices, loops;
public:
	Torus(float inner, float outer, int slices, int loops);
	void draw();
};

class Plane : public Primitiva
{
private:
	int parts;
public:
	Plane(int);
	void draw();
};

struct controlpoint
{
	float x, y, z;
	controlpoint(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
};

class Patch : public Primitiva
{
private:
	int order, partsU, partsV;
	GLenum mode;
	vector<controlpoint> controlpoints;
public:
	Patch(int, int, int, string);
	void addControlPoint(float, float, float);
	void draw();	
};

class Vehicle : public Primitiva
{
public:
	void draw();
};

class Waterline : public CGFshader, public Primitiva
{
public:
	Waterline(string heightmap, string texturemap, string fragmentshader, string vertexshader);
	void bind(float timestamp = clock());
	void setScale(float s);
	void draw();
protected:
	CGFtexture * heightMapTexture;
	CGFtexture * texture;
	GLint textureLoc, yScaleLoc, zScaleLoc, heightMapLoc, timeLoc;
	float lastTimestamp, totalTime;
	Plane *plane;
};

//Primitiva semelhante à Waterline, em que apenas muda o tamanho da area à qual aplicar o shader
class Mountain : public Waterline 
{
public:
	Mountain(string heightmap, string texturemap, string fragmentshader, string vertexshader);
	void draw();
};

class Piece : public Primitiva
{
private:
	Sphere *piece;	
	LinearAnimation *animation;
	static CGFappearance *white;
	static CGFappearance *black;
public:
	char color;
	bool placed;
	int x, y;
	Piece();
	void draw();
	void moveTo(int x, int y);
	void place(char color, int x, int y);
};

class Tabuleiro : public Primitiva
{
private:
	unsigned int size;
	Rectangle *boardFace;
	GLuint boardListID, hotspotsListID;
public:
	Tabuleiro(unsigned int size);
	void createLists();
	void draw();
	void drawHotspots();
	void drawPieces();
	std::array<Piece, 36> boardRepresentation;
	const string getBoardList(bool pieceIDs = false);
	void rotateQuadrant(Socket* socket, int quadrant, int direction);
};