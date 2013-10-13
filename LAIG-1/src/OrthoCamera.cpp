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
	glOrtho(this->left, this->right, this->bottom, this->top, this->near, this->far);
}
