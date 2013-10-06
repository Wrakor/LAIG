#include <GL/glut.h>
#include "CGFCamera.h"

class OrthoCamera : public CGFcamera
{
private:
	GLdouble near, far, left, right, top, bottom;
public:
	OrthoCamera(GLdouble near, GLdouble far, GLdouble left, GLdouble right, GLdouble top, GLdouble bottom);
	void updateProjectionMatrix(int width, int height);
	//void applyView();
};