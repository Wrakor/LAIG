#include "Light.h"
#include <iostream>
#define CG_GLIGHT_DEFAULT_RADIUS 0.2
#define CG_GLIGHT_DEFAULT_SLICES 8
#define CG_GLIGHT_DEFAULT_STACKS 8

using std::string;

int Light::getID()
{
	return this->id;
}

Light::Light(string nodeID, unsigned int lightid, float *pos, float *dir, GLfloat exp):CGFlight(lightid, pos, dir) //call parent constructor
{
	if(dir==NULL)
		this->spot = false;
	else
		this->spot = true;
	this->exp = exp;
	this->nodeID = nodeID;
	if(spot)
		glLightf(id, GL_SPOT_EXPONENT, exp);
};

void Light::update() {
	if (enabled)
		glEnable(id);
	else
		glDisable(id);

	//apenas o position e o spot_direction precisam de ser actualizados

	glLightfv(id, GL_POSITION, position);

	if(spot)
		glLightfv(id, GL_SPOT_DIRECTION, direction);
}

bool Light::isEnabled()
{
	return enabled;
}
unsigned int Light::getLightID()
{
	return id;
}
