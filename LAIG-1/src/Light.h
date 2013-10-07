#ifndef LIGHT_H
#define LIGHT_H
#include "CGFlight.h"

class Light : public CGFlight {
public:
	Light(bool spot, unsigned int lightid, float *pos, float *dir=NULL, GLfloat exp=0);
	int getID();
	void update();
	void draw();
private:
	GLfloat exp;
	bool spot;
};

#endif