#include "ExampleObject.h"

void ExampleObject::draw() 
{

	glBegin(GL_QUADS);
		glTexCoord2d(0,0);
		glVertex3d(0,0,0);
		glTexCoord2d(1,0);
		glVertex3d(4,0,0);
		glTexCoord2d(1,1);
		glVertex3d(4,3,0);
		glTexCoord2d(0,1);
		glVertex3d(0,3,0);

	glEnd();
}