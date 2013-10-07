#include <iostream>
#include "PerspectiveCamera.h"

using std::string;

PerspectiveCamera::PerspectiveCamera(string nodeID, GLdouble near, GLdouble far, float angle)
{
	this->nodeID = nodeID;
	this->near = near;
	this->far = far;
	this->angle = angle;
}

void PerspectiveCamera::updateProjectionMatrix(int width, int height)
{
	float aspect= (float)width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle,aspect, near, far);
	//glFrustum(-aspect*.04, aspect*.04, -.04, .04, this->near, this->far);
}

void PerspectiveCamera::setTargetX(float value)
{
	target[0]=value;
}
void PerspectiveCamera::setTargetY(float value)
{
	target[1]=value;
}
void PerspectiveCamera::setTargetZ(float value)
{
	target[2]=value;
}
void PerspectiveCamera::applyView()
{
	gluLookAt(position[0],position[1],position[2],target[0],target[1],target[2],0,1,0);
	glTranslatef(-position[0],-position[1],-position[2]);
	glRotatef(rotation[0], 1.f, 0.f, 0.f);
	glRotatef(rotation[1], 0.f, 1.f, 0.f);
	glRotatef(rotation[2], 0.f, 0.f, 1.f);
}