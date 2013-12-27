#pragma once

#include <vector>
#include <array>
#include <ctime>

using namespace std;

class Animation
{
protected:
	unsigned long startTime;
	bool begun, ended;
public:
	static bool animationRunning;
	string nodeID;
	Animation(string nodeID);
	virtual void init(float timestamp);
	virtual void update(float timestamp) = 0;
	virtual void draw() = 0;
};

class LinearAnimation : public Animation
{
	vector<array<float, 3>> controlPoints;
	unsigned int currentControlPoint;
	array<float, 3> currentPos;
	float span, lastTimestamp, timePerControlPoint, timeInThisControlPoint, angle, deltaX, deltaY, deltaZ;
	void calculateAngle();
	void update(float timestamp = clock());
public:
	LinearAnimation(string nodeID, float span);
	void addControlPoint(array<float, 3>);
	void init(float timestamp = clock());
	void draw();
	unsigned int getNumControlPoints();
	void changeControlPoint(unsigned int cp);
};

class CameraAnimation : public Animation
{
	array<float, 3> destination;
	float span, lastTimestamp, deltaX, deltaY, deltaZ;
public:
	CameraAnimation(array<float, 3> source, array<float, 3> destination, float span);
	void init(float timestamp = clock());
	array<float, 3> currentPos;
	void update(float timestamp = clock());
	void draw();
};