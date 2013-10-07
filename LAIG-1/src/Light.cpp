#include "Light.h"
#include <iostream>
#define CG_GLIGHT_DEFAULT_RADIUS 0.2
#define CG_GLIGHT_DEFAULT_SLICES 8
#define CG_GLIGHT_DEFAULT_STACKS 8

int Light::getID()
{
	return this->id;
}

Light::Light(bool spot, unsigned int lightid, float *pos, float *dir, GLfloat exp):CGFlight(lightid, pos, dir) //call parent constructor
{
	this->spot = spot;
	this->exp = exp;
};

void Light::update() {
	if (enabled)
		glEnable(id);
	else
		glDisable(id);

	//is this needed?
	glLightfv(id, GL_AMBIENT, ambient);
	glLightfv(id, GL_DIFFUSE, diffuse);
	glLightfv(id, GL_SPECULAR, specular);
	glLightfv(id, GL_POSITION, position);
	if(spot)
	{
		glLightfv(id, GL_SPOT_DIRECTION, direction);
		glLightf(id, GL_SPOT_EXPONENT, exp);
	}
}

void Light::draw() {
	update();

	material->apply();
	glPushMatrix();
		glTranslatef(position[0],position[1],position[2]);
		gluSphere(glu_quadric, CG_GLIGHT_DEFAULT_RADIUS, CG_GLIGHT_DEFAULT_SLICES, CG_GLIGHT_DEFAULT_STACKS);
	glPopMatrix();
}

bool Light::isEnabled()
{
	return enabled;
}