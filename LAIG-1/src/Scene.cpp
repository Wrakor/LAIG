#include "Scene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"

#include <math.h>
#include <iostream>
#include <typeinfo>

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
	createDisplayLists(this->rootNode);
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
		((Light *)(*it))->draw();

	// Draw axis
	axis.draw();


	// ---- END Background, camera and axis setup
	processGraph(rootNode); //temos de passar o id do nó inicial
	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
	//std::this_thread::sleep_for(std::chrono::milliseconds(17));
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

void Scene::addNode(string nodeID, Node *node)
{
	this->nodes.insert(std::pair<string, Node *>(nodeID, node));
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

Light* Scene::getLightByGLFloat(GLfloat id)
{
	std::list<CGFlight *>::iterator it = scene_lights.begin();

	for(;it!=scene_lights.end();++it)
		if ( ((Light*)(*it))->getLightID() == id)
			return (Light *)(*it);
	throw "Light not found";
}

void Scene::setDrawMode(GLenum drawMode)
{
	this->drawMode = drawMode;
}

void Scene::processGraph(string nodeID, Appearance *app)
{
	Node *node = nodes[nodeID];
	if(node->appearance) //se nó tem aparência, substitui a do pai
		app=node->appearance;

	if(app) //se a aparência não for nula
		app->apply(); //aplica a aparência para desenhar as primitivas
	else
		(new CGFappearance())->apply(); //material por defeito

	if (node->displayList) //se houver uma display list, chama-a e ignora filhos
		glCallList(node->displayListID);
	else //se não, percorre normalmente
	{
		glMultMatrixf(node->T);
		for (vector<Primitiva *>::iterator it = node->primitivas.begin(); it != node->primitivas.end(); it++)
			(*it)->draw();

		for (vector<string>::iterator it = node->children.begin(); it != node->children.end(); it++)
		{
			glPushMatrix();
			processGraph(*it, app);
			glPopMatrix();
		}
	}
}

void Scene::createDisplayLists(string nodeID, Appearance *app, bool onDisplayList)
{
	Node *node = nodes[nodeID];
	bool createdDisplayList = false;
	if (node->displayList && !onDisplayList && node->displayListID == NULL) //se nó tem uma displaylist que ainda não foi criada, e não estamos a construir uma displaylist
	{
		onDisplayList = true;
		createdDisplayList = true;
		node->displayListID = glGenLists(1);
		glNewList(node->displayListID, GL_COMPILE);
	}
	if (onDisplayList) //só desenha se estivermos dentro de uma DL
	{
		glMultMatrixf(node->T);
		if (node->appearance) //se nó tem aparência, substitui a do pai
			app = node->appearance;

		if (node->primitivas.size() > 0) //se nó tem primitivas para desenhar
		{
			if (app) //se a aparência não for nula
				app->apply(); //aplica a aparência para desenhar as primitivas
		}

		for (vector<Primitiva *>::iterator it = node->primitivas.begin(); it != node->primitivas.end(); it++)
			(*it)->draw();
	}
	for (vector<string>::iterator it = node->children.begin(); it != node->children.end(); it++)
	{
		glPushMatrix();
		createDisplayLists(*it, app, onDisplayList);
		glPopMatrix();
	}
	if (createdDisplayList) //se este foi o nó onde a DL actual foi criada, fecha-a
	{
		glEndList();
		//percorre novamente filhos para criar DL's individualmente
		for (vector<string>::iterator it = node->children.begin(); it != node->children.end(); it++)
		{
			glPushMatrix();
			createDisplayLists(*it);
			glPopMatrix();
		}
	}
}