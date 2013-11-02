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
}

void LinearAnimation::addControlPoint(array<float, 3> controlPoint)
{
	this->controlPoints.push_back(controlPoint);
}

void LinearAnimation::update(float timestamp)
{
	float animationTime = (timestamp - startTime)/CLOCKS_PER_SEC;
	float timeSinceLastUpdate = (timestamp - lastTimestamp) / CLOCKS_PER_SEC;
	if (currentControlPoint != controlPoints.size()-1)
	{
		currentPos[0] += (controlPoints[currentControlPoint + 1][0] - controlPoints[currentControlPoint][0]) * (timeSinceLastUpdate / span * (controlPoints.size() - 1));
		currentPos[1] += (controlPoints[currentControlPoint + 1][1] - controlPoints[currentControlPoint][1]) * (timeSinceLastUpdate / span * (controlPoints.size() - 1));
		currentPos[2] += (controlPoints[currentControlPoint + 1][2] - controlPoints[currentControlPoint][2]) * (timeSinceLastUpdate / span * (controlPoints.size() - 1));
		//if ((floorf(animationTime*1000) == floorf((span / (controlPoints.size() - 1))*1000)) && currentControlPoint < (controlPoints.size() - 2)) //NÃO ESTÁ BEM, arranjar arredondamento
		if ((floorf(currentPos[0]*100) == floorf(controlPoints[currentControlPoint + 1][0]*100)) && (floorf(currentPos[1]*100) == floorf(controlPoints[currentControlPoint + 1][1]*100)) && (floorf(currentPos[2]*100) == floorf(controlPoints[currentControlPoint + 1][2]*100)))
			currentControlPoint++;
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