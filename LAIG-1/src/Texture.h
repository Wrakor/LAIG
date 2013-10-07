#ifndef TEXTURE_H
#define TEXTURE_H
#include "CGFtexture.h"

class Texture : public CGFtexture
{
public:
	Texture(string nodeID, string tex):CGFtexture(tex){
		this->nodeID = nodeID;
	}
	string nodeID;
};

#endif