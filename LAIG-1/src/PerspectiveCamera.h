#pragma once

#include <GL/glut.h>
#include "CGFCamera.h"

using std::string;

class PerspectiveCamera : public CGFcamera
{
private:
	float angle;
	GLdouble near, far;
public:
	PerspectiveCamera(string nodeID, GLdouble near, GLdouble far, float angle);
	void updateProjectionMatrix(int width, int height);
	void setTargetX(float value);
	void setTargetY(float value);
	void setTargetZ(float value);
	void applyView();
	string nodeID;
};