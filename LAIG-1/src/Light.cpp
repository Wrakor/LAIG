#include "Light.h"

int Light::getID()
{
	return this->id;
}

Light::Light(unsigned int lightid, float *pos, float *dir):CGFlight(lightid, pos, dir) //call parent constructor
{
};