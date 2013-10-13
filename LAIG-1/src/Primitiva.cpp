#include "Primitiva.h"
using namespace std;

Rectangle::Rectangle(float x1,float x2,float y1,float y2)
{
	this->x1=x1;
	this->x2=x2;
	this->y1=y1;
	this->y2=y2;
}

void Rectangle::draw(){
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0);
		glVertex2f(x1,y1);
		glTexCoord2f(1,0);
		glVertex2f(x2,y1);
		glTexCoord2f(1,1);
		glVertex2f(x2,y2);
		glTexCoord2f(0,1);
		glVertex2f(x1,y2);
	glEnd();
}

void Triangle::calculateTexCoords()
{
	float width, height;
	float angle;
	float dotProduct;

	float vecAC_x, vecAC_y, vecAC_z, vecAB_x, vecAB_y, vecAB_z;

	vecAC_x = x3-x1;
	vecAC_y = y3-y1;
	vecAC_z = z3-z1;

	vecAB_x = x2-x1;
	vecAB_y = y2-y1;
	vecAB_z = z2-z1;

	dotProduct = (vecAC_x)*(vecAB_x) + (vecAC_y)*(vecAB_y) + (vecAC_z)*(vecAB_z);

	lengthAC = sqrt((vecAC_x)*(vecAC_x) + (vecAC_y)*(vecAC_y) + (vecAC_z)*(vecAC_z));
	lengthAB = sqrt((vecAB_x)*(vecAB_x) + (vecAB_y)*(vecAB_y) + (vecAB_z)*(vecAB_z));

	angle = acos(dotProduct/(lengthAB*lengthAC));

	width = cos(angle)*lengthAC;
	height = sin(angle)*lengthAC;

	texCoord_x = width;
	texCoord_y = height;
}

Triangle::Triangle(float x1,float x2,float x3,float y1, float y2, float y3, float z1, float z2, float z3)
{
	this->x1=x1;
	this->x2=x2;
	this->x3=x3;
	this->y1=y1;
	this->y2=y2;
	this->y3=y3;
	this->z1=z1;
	this->z2=z2;
	this->z3=z3;

	normal_x = 0;
	normal_y = 0;
	normal_z = 0;

	normal_x += (y1-y2)*(z1-z2);
	normal_y += (z1-z2)*(x1-x2);
	normal_z += (x1-x2)*(y1-y2);

	normal_x += (y2-y3)*(z2-z3);
	normal_y += (z2-z3)*(x2-x3);
	normal_z += (x2-x3)*(y2-y3);

	normal_x += (y3-y1)*(z3-z1);
	normal_y += (z3-z1)*(x3-x1);
	normal_z += (x3-x1)*(y3-y1);

	calculateTexCoords();
}

void Triangle::draw()
{
	glBegin(GL_TRIANGLES);
		glNormal3f(normal_x, normal_y, normal_z);
		glTexCoord2d(0,0);
		glVertex3f(x1, y1, z1);
		glTexCoord2d(lengthAB,0);
        glVertex3f(x2, y2, z2);
		glTexCoord2d(texCoord_x,texCoord_y);
        glVertex3f(x3, y3, z3);
	glEnd();
}

Cylinder::Cylinder(float base, float top, float height, int slices, int stacks)
{
	this->base = base;
	this->top = top;
	this->height = height;
	this->slices = slices;
	this->stacks = stacks;
}

void Cylinder::draw()
{
	GLUquadric *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, true);
	gluCylinder(quadric, base, top, height, slices, stacks);
	glRotatef(180, 1, 0, 0);
	gluDisk(quadric, 0, base, slices, 1);
	glRotatef(180, 1, 0, 0);
	glTranslatef(0, 0, height);
	gluDisk(quadric, 0, top, slices, 1);
	glTranslatef(0, 0, -height);
	gluDeleteQuadric(quadric);
}

Sphere::Sphere(float radius, int slices, int stacks)
{
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;
}

void Sphere::draw()
{
	GLUquadric *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, true);
	gluSphere(quadric, radius, slices, stacks);
	gluDeleteQuadric(quadric);
}

Torus::Torus(float inner, float outer, int slices, int loops)
{
	this->inner = inner;
	this->outer = outer;
	this->slices = slices;
	this->loops = loops;
}

void Torus::draw()
{

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidTorus(inner, outer, slices, loops); //não gera coordenadas de textura
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}