#include "Appearance.h"

Appearance::Appearance(string nodeID, float* ambient, float* diffuse, float* specular, float* emissive, float shininess):CGFappearance(ambient,diffuse,specular,shininess)
{
    this->emissive[0] = emissive[0];
	this->emissive[1] = emissive[1];
	this->emissive[2] = emissive[2];
	this->emissive[3] = emissive[3];
	this->nodeID = nodeID;
}

void Appearance::apply() 
{
	glDisable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
	if (texture!=NULL)
	{
		glEnable(GL_TEXTURE_2D);
		texture->apply();	       
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
	}
	else
		glDisable(GL_TEXTURE_2D);	       
}