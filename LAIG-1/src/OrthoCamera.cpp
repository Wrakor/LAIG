#include <iostream>
#include "OrthoCamera.h"

using std::string;

OrthoCamera::OrthoCamera(string nodeID, GLdouble near, GLdouble far, GLdouble left, GLdouble right, GLdouble top, GLdouble bottom):Camera(nodeID, near, far)
{
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->setX(0);
	this->setY(0);
	this->setZ(0);
}

void OrthoCamera::updateProjectionMatrix(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(this->left, this->right, this->bottom, this->top, ((Camera *)this)->near, ((Camera *)this)->far);
}

/*void OrthoCamera::applyView()
{
	glRotatef(rotation[0], 1.f, 0.f, 0.f);
	glRotatef(rotation[1], 0.f, 1.f, 0.f);
	glRotatef(rotation[2], 0.f, 0.f, 1.f);
}*/