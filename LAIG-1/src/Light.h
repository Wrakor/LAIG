#ifndef LIGHT_H
#define LIGHT_H
#include "CGFlight.h"

class Light : public CGFlight {
public:
	Light(unsigned int lightid, float *pos, float *dir=NULL);
	int getID();
};

#endif