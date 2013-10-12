#pragma once

#include "CGFcamera.h"
#include <string>
#include <GL/glut.h>

using std::string;

class Camera : public CGFcamera
{
public:
	Camera(string nodeID, GLdouble near, GLdouble far);
	string nodeID;
	GLdouble near, far;
};
