#include <iostream>
#include "PerspectiveCamera.h"

using std::string;

PerspectiveCamera::PerspectiveCamera(string nodeID, GLdouble near, GLdouble far, float angle):Camera(nodeID, near, far)
{
	this->angle = angle;
}

void PerspectiveCamera::updateProjectionMatrix(int width, int height)
{
	float aspect= (float)width / (float)height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle,aspect, near, far);
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
	/*glRotatef(rotation[0], 1.f, 0.f, 0.f);
	glRotatef(rotation[1], 0.f, 1.f, 0.f);
	glRotatef(rotation[2], 0.f, 0.f, 1.f);*/
	if (this->animation != NULL)
	{
		this->animation->update();
		this->position[0] = this->animation->currentPos[0];
		this->position[1] = this->animation->currentPos[1];
		this->position[2] = this->animation->currentPos[2];
	}
	else
		reset(); //dont allow moving camera
	gluLookAt(position[0],position[1],position[2],target[0],target[1],target[2],0,1,0);
}

void PerspectiveCamera::createCameraAnimation(array<float, 3> destination)
{
	array<float, 3> position = this->getPosition();
	this->animation = NULL;
	this->animation = new CameraAnimation(destination, position, 1); //start at previous camera location and go to the current camera's position
	this->animation->init();
}

array<float, 3> PerspectiveCamera::getPosition()
{
	array<float, 3> position = { this->originalPosition[0], this->originalPosition[1], this->originalPosition[2] };
	return position;
}