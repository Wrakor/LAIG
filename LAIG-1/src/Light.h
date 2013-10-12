#pragma once

#include "CGFlight.h"

using std::string;

class Light : public CGFlight {
public:
	Light(string nodeID, unsigned int lightid, float *pos, float *dir=NULL, GLfloat exp=0);
	int getID();
	void update();
	bool isEnabled();
	string nodeID;
	unsigned int getLightID();
private:
	GLfloat exp;
	bool spot;
};
