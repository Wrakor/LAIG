#pragma once
#include "CGFapplication.h" //TEM DE ESTAR AQUI SENÃO ESTOURA
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

