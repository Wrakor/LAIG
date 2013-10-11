#pragma once
#include "CGFtexture.h"

class Texture : public CGFtexture
{
public:
	Texture(string nodeID, string fileName);
	string nodeID;
};
