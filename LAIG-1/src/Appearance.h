#pragma once
#include "CGFapplication.h"
#include "CGFappearance.h"

class Appearance : public CGFappearance
{
private:
    float emissive[4];

public:
	Appearance(string nodeID, float* ambient, float* diffuse, float* specular, float* emissive, float shininess);
	string nodeID;
	void apply();
};

