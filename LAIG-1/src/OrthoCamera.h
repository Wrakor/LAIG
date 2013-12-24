#pragma once
#include <GL/glut.h>
#include "Camera.h"

using std::string;

class OrthoCamera : public Camera
{
private:
	GLdouble left, right, top, bottom;
public:
	OrthoCamera(string nodeID, GLdouble near, GLdouble far, GLdouble left, GLdouble right, GLdouble top, GLdouble bottom);
	void updateProjectionMatrix(int width, int height);
};