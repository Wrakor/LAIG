#include <iostream>
#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(GLdouble near, GLdouble far, float angle)
{
	this->near = near;
	this->far = far;
	this->angle = angle;
	this->mode = TARGET_MODE; //perspective cameras always look to the target?
	this->rotation[0]=0;
	this->rotation[1]=-45;
	this->rotation[2]=0;
	//this->rotate(CG_CGFcamera_AXIS_Y, -45);

}

void PerspectiveCamera::updateProjectionMatrix(int width, int height)
{
	float aspect= (float)width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-aspect*.04, aspect*.04, -.04, .04, this->near, this->far);
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
	
	glRotatef(rotation[0], 1.f, 0.f, 0.f);
	glRotatef(rotation[1], 0.f, 1.f, 0.f);
	glRotatef(rotation[2], 0.f, 0.f, 1.f);
	glTranslatef(-position[0],-position[1],-position[2]);
	//gluLookAt(position[0],position[1],position[2],target[0],target[1],target[2],0,1,0);
}