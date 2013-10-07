#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "CGFscene.h"
#include "CGFshader.h"
#include "Light.h"

class DemoScene : public CGFscene
{
public:
	DemoScene();
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
	void initCameras();
	bool lightingDoubleSided, lightingLocal, lightingEnabled;
	void addLight(Light *l);
private:
	//CGFlight* light0;
	CGFobject* obj;
	CGFappearance* materialAppearance;
	CGFappearance* textureAppearance;
	//CGFshader* shader;
};

#endif