#pragma once
#include "GL/glut.h"

class Primitiva{
public:
	virtual void draw()=0;
};

class Rectangle : public Primitiva
{
private:
	float x1,x2,y1,y2;
public:
	Rectangle(float x1,float x2,float y1,float y2);
	void draw();
};

class Triangle : public Primitiva
{
private:
	float x1,x2,x3,y1,y2,y3,z1,z2,z3;
public:
	Triangle(float x1,float x2,float x3,float y1, float y2, float y3, float z1, float z2, float z3);
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