#ifndef Scene_H
#define Scene_H

#include "CGFscene.h"
#include "CGFshader.h"
#pragma once

#include "Light.h"
#include "Texture.h"
#include "Appearance.h"

class Scene : public CGFscene
{
public:
	Scene();
	void init();
	void display();
	void update(unsigned long t);
	//~Scene();
	GLenum drawMode;
	GLenum shadeModel;
	bool cullface;
	GLenum cullfaceMode;
	GLenum cullorder;
	int addCamera(CGFcamera *c);
	void initCameras();
	bool lightingDoubleSided, lightingLocal, lightingEnabled;
	void addLight(Light *l);
	void addTexture(Texture* texture);
	void addAppearance(Appearance* appearance);
	Texture* getTextureByID(string nodeID);
	Appearance* getAppearanceByID(string nodeID);
	void loadTextures();
private:
	//CGFlight* light0;
	CGFobject* obj;
	//CGFappearance* materialAppearance;
	//CGFappearance* textureAppearance;
	vector<Texture*> textures;
	vector<Appearance*> appearances;
	//CGFshader* shader;
};

#endif