#pragma once

#include <GL/glut.h>
#include "Camera.h"
#include "Animation.h"

using std::string;

class PerspectiveCamera : public Camera
{
private:
	float angle;
public:
	PerspectiveCamera(string nodeID, GLdouble near, GLdouble far, float angle);
	void updateProjectionMatrix(int width, int height);
	void setTargetX(float value);
	void setTargetY(float value);
	void setTargetZ(float value);
	void applyView();
	void createCameraAnimation(array<float, 3> destination);
	array<float, 3> getPosition();
	string nodeID;
};