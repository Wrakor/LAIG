#include "Camera.h"

Camera::Camera(string nodeID, GLdouble near, GLdouble far)
{
	this->nodeID = nodeID;
	this->near = near;
	this->far = far;
}