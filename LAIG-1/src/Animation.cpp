#include "Animation.h"
#include <GL/glut.h>

using namespace std;

Animation::Animation(string nodeID)
{
	this->nodeID = nodeID;
	this->startTime = 0;
}

void Animation::init(float timestamp)
{
	this->startTime = timestamp;
}

LinearAnimation::LinearAnimation(string nodeID, float span) : Animation(nodeID)
{
	this->span = span;
	this->currentControlPoint = 0;
}

void LinearAnimation::init(float timestamp)
{
	Animation::init(timestamp);
	this->currentPos = controlPoints[0];
	this->lastTimestamp = timestamp;
	this->timePerControlPoint = span / (controlPoints.size() - 1);
	this->timeInThisControlPoint = 0;
}

void LinearAnimation::addControlPoint(array<float, 3> controlPoint)
{
	this->controlPoints.push_back(controlPoint);
}

void LinearAnimation::update(float timestamp)
{
	float animationTime = (timestamp - startTime)/CLOCKS_PER_SEC;
	float timeSinceLastUpdate = (timestamp - lastTimestamp) / CLOCKS_PER_SEC;
	this->timeInThisControlPoint += timeSinceLastUpdate;
	if (animationTime<span) //if animation isn't done
	{
		if (timeInThisControlPoint>timePerControlPoint) //if this control point is done, go to the next
		{
			currentControlPoint++;
			this->timeInThisControlPoint = 0;
		}
		else
		{
			//increment current position: distance to cover for this control point * percentage of time elapsed 
			currentPos[0] += (controlPoints[currentControlPoint + 1][0] - controlPoints[currentControlPoint][0]) * (timeSinceLastUpdate / timePerControlPoint); //x
			currentPos[1] += (controlPoints[currentControlPoint + 1][1] - controlPoints[currentControlPoint][1]) * (timeSinceLastUpdate / timePerControlPoint); //y
			currentPos[2] += (controlPoints[currentControlPoint + 1][2] - controlPoints[currentControlPoint][2]) * (timeSinceLastUpdate / timePerControlPoint); //z
		}
	}
	else //reset
	{
		currentControlPoint = 0;
		init();
	}
	lastTimestamp = timestamp;
}

void LinearAnimation::draw()
{
	update();
	glTranslatef(currentPos[0], currentPos[1], currentPos[2]);
}