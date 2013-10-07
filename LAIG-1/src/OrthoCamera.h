#include <GL/glut.h>
#include "CGFCamera.h"

using std::string;

class OrthoCamera : public CGFcamera
{
private:
	GLdouble near, far, left, right, top, bottom;
public:
	OrthoCamera(string nodeID, GLdouble near, GLdouble far, GLdouble left, GLdouble right, GLdouble top, GLdouble bottom);
	void updateProjectionMatrix(int width, int height);
	//void applyView();
	string nodeID;
};