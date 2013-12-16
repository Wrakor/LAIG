#pragma once
#include "CGFscene.h"
#include "CGFshader.h"

#include "Light.h"
#include "Texture.h"
#include "Appearance.h"
#include "Node.h"
#include "Camera.h"
#include "Animation.h"
#include "Socket.h"
#include <map>

#define WHITE 0
#define BLACK 1

#define CONNECTING 0
#define PLACEPIECE 1
#define ROTATE 2
#define GAMEOVER 3

class Scene : public CGFscene
{
public:
	Scene();
	void init();
	void display();
	~Scene();
	GLenum drawMode, shadeModel, cullfaceMode, cullorder;
	bool cullface;
	GLfloat backgroundR, backgroundG, backgroundB, backgroundA;
	int addCamera(Camera *c);
	void initCameras();
	bool lightingDoubleSided, lightingLocal, lightingEnabled;
	void addLight(Light *l);
	void addTexture(Texture* texture);
	void addAppearance(Appearance* appearance);
	void addAnimation(Animation* animation);
	Texture* getTextureByID(string nodeID);
	Appearance* getAppearanceByID(string nodeID);
	Animation* getAnimationByID(string nodeID);
	void addNode(string nodeID, Node* node);
	unsigned int getNumLights();
	unsigned int getNumCameras();
	Camera* getCamera(unsigned int id);
	unsigned int getPositionInCamerasVector(string ID); //quero a posição do vector de câmaras dando o ID do yaf
	Camera *getActiveCamera();
	Light *getLightByGLFloat(GLfloat id);
	void setDrawMode(GLenum drawMode);
	void processGraph(string nodeID, Appearance *app=NULL);
	void createDisplayLists(string nodeID, Appearance *app=NULL, bool onDisplayList=false);
	string rootNode; //id do primeiro nó
	void initAnimations();
	bool runAnimations;
	Tabuleiro *board;
	void placePiece(unsigned int pos);
	unsigned int gameState;
	Socket *socket;
	void setGameMessage(string text);
private:
	vector<Texture*> textures;
	vector<Appearance*> appearances;
	vector<Animation*> animations;
	map<string, Node*> nodes;
	unsigned int player;
};