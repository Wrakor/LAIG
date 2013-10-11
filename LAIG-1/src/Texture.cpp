#include "Texture.h"

Texture::Texture(string nodeID, string fileName):CGFtexture(fileName){
		this->nodeID = nodeID;
	}