#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "CGFscene.h"
#include "CGFshader.h"

class DemoScene : public CGFscene
{
public:
	void init();
	void display();
	void update(unsigned long t);
	//~DemoScene();
	GLenum drawMode;
	GLenum shadeModel;
	bool cullface;
	GLenum cullfaceMode;
	GLenum cullorder;
	int addCamera(CGFcamera *c);
private:
	CGFlight* light0;
	CGFobject* obj;
	CGFappearance* materialAppearance;
	CGFappearance* textureAppearance;
	//CGFshader* shader;
};

#endif