#pragma once

#include "CGFcamera.h"
#include <string>
#include <GL/glut.h>

using std::string;

class Camera : public CGFcamera
{
private:
	float originalPosition[3]; //to enable camera reset
public:
	Camera(string nodeID, GLdouble near, GLdouble far);
	string nodeID;
	GLdouble near, far;
	void reset();
	void setX(float value);
	void setY(float value);
	void setZ(float value);
};
