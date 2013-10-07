#include "Texture.h"

Texture::Texture(string nodeID, string fileName):CGFtexture(){ //can't load textures on the beggining, scene must be initiated
		this->nodeID = nodeID;
		this->texName = fileName;
	}

void Texture::load(){
	loadTexture(texName);
}