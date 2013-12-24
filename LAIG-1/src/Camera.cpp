#include "Camera.h"

Camera::Camera(string nodeID, GLdouble near, GLdouble far)
{
	this->nodeID = nodeID;
	this->near = near;
	this->far = far;
	this->animation = NULL;
}

void Camera::reset()
{
	this->position[0] = originalPosition[0];
	this->position[1] = originalPosition[1];
	this->position[2] = originalPosition[2];

	this->rotation[0] = 0;
	this->rotation[1] = 0;
	this->rotation[2] = 0;
}

void Camera::setX(float value)
{
	CGFcamera::setX(value);
	this->originalPosition[0] = value;
}
void Camera::setY(float value)
{
	CGFcamera::setY(value);
	this->originalPosition[1] = value;
}
void Camera::setZ(float value)
{
	CGFcamera::setZ(value);
	this->originalPosition[2] = value;
}