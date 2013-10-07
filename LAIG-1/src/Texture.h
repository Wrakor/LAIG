#ifndef TEXTURE_H
#define TEXTURE_H
#include "CGFtexture.h"

class Texture : public CGFtexture
{
public:
	Texture(string nodeID, string fileName);
	string nodeID;
	void load();
};

#endif