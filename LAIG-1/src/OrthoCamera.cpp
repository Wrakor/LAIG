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

void OrthoCamera::createCameraAnimation(array<float, 3> destination)
{

}
array<float, 3> OrthoCamera::getPosition()
{
	array<float, 3> position = { 0 };
	return position;
}