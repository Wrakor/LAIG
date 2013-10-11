#pragma once
#include "CGFscene.h"
#include "CGFshader.h"

#include "Light.h"
#include "Texture.h"
#include "Appearance.h"
#include "Node.h"
#include "Camera.h"
#include <map>

class Scene : public CGFscene
{
public:
	Scene();
	void init();
	void display();
	void update(unsigned long t);
	//~Scene();
	GLenum drawMode, shadeModel, cullfaceMode, cullorder;
	bool cullface;
	GLfloat backgroundR, backgroundG, backgroundB, backgroundA;
	int addCamera(Camera *c);
	void initCameras();
	bool lightingDoubleSided, lightingLocal, lightingEnabled;
	void addLight(Light *l);
	void addTexture(Texture* texture);
	void addAppearance(Appearance* appearance);
	Texture* getTextureByID(string nodeID);
	Appearance* getAppearanceByID(string nodeID);
	void addNode(string nodeID, Node* node);
	unsigned int getNumLights();
	unsigned int getNumCameras();
	Camera* getCamera(unsigned int id);
	unsigned int getPositionInCamerasVector(string ID); //quero a posição do vector de câmaras dando o ID do yaf
	Camera *getActiveCamera();
	//std::list<CGFlight*> getSceneLights();
	Light *getLightByGLFloat(GLfloat id);
	void setDrawMode(GLenum drawMode);
	void processGraph(string nodeID);
	string rootNode; //id do primeiro nó

private:
	CGFobject* obj;
	vector<Texture*> textures;
	vector<Appearance*> appearances;
	map<string, Node*> nodes;
	CGFshader* shader;
};