#include <iostream>
#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(GLdouble near, GLdouble far, GLdouble left, GLdouble right, GLdouble top, GLdouble bottom)
{
	this->near = near;
	this->far = far;
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->setX(0);
	this->setY(0);
	this->setZ(0);
	//this->setWalkMode();
}

void OrthoCamera::updateProjectionMatrix(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(this->left, this->right, this->bottom, this->top, this->near, this->far);
}

/*void OrthoCamera::applyView()
{
	glRotatef(rotation[0], 1.f, 0.f, 0.f);
	glRotatef(rotation[1], 0.f, 1.f, 0.f);
	glRotatef(rotation[2], 0.f, 0.f, 1.f);
}*/