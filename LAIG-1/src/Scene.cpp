#include "Scene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"

#include "ExampleObject.h"

#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <typeinfo>

#include "CGFappearance.h"

Scene::Scene(){
	this->scene_cameras.clear(); //limpar vector de câmaras (estão a ser criadas no constructor do pai (init cameras)
}

void Scene::init() 
{
	//limpa a cor activa debackground e define nova cor RGBA
	glClearColor(backgroundR,backgroundG,backgroundB,backgroundA);

	// Enables lighting computations
	if(this->lightingEnabled)
		glEnable(GL_LIGHTING);

	// Sets up some lighting parameters
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, this->lightingDoubleSided);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, this->lightingLocal);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light::background_ambient);  // Define ambient light

	//Define shading
	glShadeModel(this->shadeModel);

	//Define cullface
	if(this->cullface)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(this->cullfaceMode);
	}
	else
		glDisable(GL_CULL_FACE);

	//Define cullorder
	glFrontFace(this->cullorder);

	cout << "CAMERAS: " << endl;
	for(unsigned int i = 0;i<this->scene_cameras.size();i++)
		cout << i << endl;

	//this->activateCamera(0);

	// cout lights
	cout << "LIGHTS: " << endl;
	std::list<CGFlight *>::iterator it = scene_lights.begin();
	for(;it!=scene_lights.end();++it)
	{
		cout << ((Light *)(*it))->getID() << endl;
	}

	// Defines a default normal
	glNormal3f(0,0,1);

	obj=new ExampleObject();
	shader=new CGFshader("data/texshader.vert","data/texshader.frag");

	loadTextures(); //got to load textures after initing scene

	setUpdatePeriod(30);
}

void Scene::update(unsigned long t)
{
	shader->bind();
	shader->update(t/400.0);
	shader->unbind();
	
}
	
void Scene::display() 
{

	// ---- BEGIN Background, camera and axis setup
	
	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//Define drawmode
	glPolygonMode(GL_FRONT_AND_BACK, this->drawMode);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();

	// Draw (and update) light
	std::list<CGFlight *>::iterator it = scene_lights.begin();
	for(;it!=scene_lights.end();++it)
	{
		((Light *)(*it))->draw();
	}

	// Draw axis
	axis.draw();


	// ---- END Background, camera and axis setup


	// ---- BEGIN feature demos

	// Simple object
	appearances[0]->apply();
	obj->draw();

	// textured object

	glTranslatef(0,4,0);
	appearances[1]->apply();
	obj->draw();

	// shader object

	glTranslatef(0,4,0);
	shader->bind();
	obj->draw();
	shader->unbind();


	// ---- END feature demos

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
	std::this_thread::sleep_for(std::chrono::milliseconds(17));
}

/*Scene::~Scene()
{
	delete(shader);
	delete(textureAppearance);
	delete(materialAppearance);
	delete(obj);
	delete(light0);
} commented since values may not exist (parse failed)*/

int Scene::addCamera(Camera *c){
	int id = this->scene_cameras.size();
	this->scene_cameras.push_back(c);
	return id;
}

void Scene::addLight(Light *l){
	this->scene_lights.push_back(l);
}

void Scene::initCameras(){
	cout << "INITIATING CAMERAS" << endl; //isto não é chamado porquê? porque não dei overload ao constructor
}

void Scene::addTexture(Texture* texture)
{
	textures.push_back(texture);
}

void Scene::addAppearance(Appearance* appearance)
{
	appearances.push_back(appearance);
}

Texture* Scene::getTextureByID(string nodeID)
{
	for(unsigned int i=0;i<textures.size();i++)
		if(textures[i]->nodeID == nodeID)
			return textures[i];
	throw "Texture not found"; //if texture isn't found, throw error
}

Appearance* Scene::getAppearanceByID(string nodeID)
{
	for(unsigned int i=0;i<appearances.size();i++)
		if(appearances[i]->nodeID == nodeID)
			return appearances[i];
	throw "Appearance not found"; //if appearance isn't found, throw error
}

void Scene::loadTextures()
{
	for(unsigned int i = 0; i<textures.size();i++)
		textures[i]->load();
}

void Scene::addNode(Node *node)
{
	this->nodes.push_back(node);
}

unsigned int Scene::getNumLights()
{
	return this->scene_lights.size();
}

unsigned int Scene::getNumCameras()
{
	return this->scene_cameras.size();
}

Camera* Scene::getCamera(unsigned int id)
{
	return (Camera *)scene_cameras[id];
}

unsigned int Scene::getPositionInCamerasVector(string ID)
{
	for(unsigned int i=0;i<this->scene_cameras.size();i++)
		if (((Camera *)scene_cameras[i])->nodeID == ID)
				return i;

	throw "Camera not found";
}

Camera* Scene::getActiveCamera()
{
	return (Camera*)activeCamera;
}

/*std::list<CGFlight*> Scene::getSceneLights()
{
	return scene_lights;
}*/

Light* Scene::getLightByGLFloat(GLfloat id)
{
	std::list<CGFlight *>::iterator it = scene_lights.begin();

	for(;it!=scene_lights.end();++it)
	{
		if ( ((Light*)(*it))->getLightID() == id)
		{
			return (Light *)(*it);
		}
	}
}

void Scene::setDrawMode(GLenum drawMode)
{
	this->drawMode = drawMode;
}