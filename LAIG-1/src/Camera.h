#pragma once

#include "CGFcamera.h"
#include "Animation.h"
#include <string>
#include <GL/glut.h>

using std::string;

class Camera : public CGFcamera
{
protected:
	float originalPosition[3]; //to enable camera reset
	CameraAnimation *animation;
public:
	Camera(string nodeID, GLdouble near, GLdouble far);
	string nodeID;
	GLdouble near, far;
	void reset();
	void setX(float value);
	void setY(float value);
	void setZ(float value);
	virtual void createCameraAnimation(array<float, 3> destination) = 0;
	virtual void updateProjectionMatrix(int width, int height) = 0;
	virtual array<float, 3> getPosition() = 0;
};
