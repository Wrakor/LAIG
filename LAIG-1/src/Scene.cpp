#include "Scene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include "Interface.h"

#include <math.h>
#include <iostream>
#include <typeinfo>

Scene::Scene(){
	this->scene_cameras.clear(); //limpar vector de câmaras (estão a ser criadas no constructor do pai (init cameras)
	this->runAnimations = true;
	this->socket = new Socket("127.0.0.1", 60070);
	this->player = PLAYERONE; //first player is white
	this->gameState = CONNECTING;
	this->gameStarted = false;
	this->playerOneName = "White";
	this->playerTwoName = "Black";
	backgroundR2 = 0.85;
	backgroundG2 = 0.82;
	backgroundB2 = 0.67;
	backgroundA2 = 1;
	this->waitForAnimations = true; //block while an animation is running
	this->inReplay = false; //if currently in a replay
	this->computerPlaying = false; //if this is a computer move
}

void Scene::init() 
{
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
	//createDisplayLists(this->rootNode);
	initAnimations();

	//start socket
	if (socket->socketConnect())
		this->gameState = PLACEPIECE;
	else
		throw "Prolog not running!";
	//glutFullScreen();
}

void Scene::display() 
{
	//limpa a cor activa debackground e define nova cor RGBA
	if (gameEnvironment==1)
		glClearColor(backgroundR, backgroundG, backgroundB, backgroundA);
	else if (gameEnvironment==2)
		glClearColor(backgroundR2, backgroundG2, backgroundB2, backgroundA2);

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//Define drawmode
	glPolygonMode(GL_FRONT_AND_BACK, this->drawMode);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();

	GLint rMode;
 
	glGetIntegerv(GL_RENDER_MODE, &rMode);

	if (rMode == GL_RENDER) //se em modo normal, desenha a cena
	{
		// Draw (and update) light
		std::list<CGFlight *>::iterator it = scene_lights.begin();
		for (; it != scene_lights.end(); ++it)
			((Light *)(*it))->draw();

		// Draw axis
		//axis.draw();
		
		// ---- END Background, camera and axis setup
		processGraph(rootNode); //temos de passar o id do nó inicial
		//board->draw();
		// We have been drawing in a memory area that is not visible - the back buffer, 
		// while the graphics card is showing the contents of another buffer - the front buffer
		// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
		//board->drawHotspots(); //Testing
	}
	if (!Animation::animationRunning || !waitForAnimations)
	{
		if (inReplay)
			replay();
		else if (!computerPlaying) //only draw arrows and hotspots if it's a human player's turn
		{
			if (rMode == GL_SELECT && gameState == PLACEPIECE && gameStarted) //se em modo de pick, desenha os hotspots
				board->drawHotspots();

			if (gameState == ROTATE) //in all render modes
				board->drawArrows(player);
		}
		else //if the computer is playing, start the rotation
			computerPlay();
	}
	glutSwapBuffers();
	//std::this_thread::sleep_for(std::chrono::milliseconds(17));
}

Scene::~Scene()
{
	delete(socket); //close socket
}

int Scene::addCamera(Camera *c){
	int id = this->scene_cameras.size();
	this->scene_cameras.push_back(c);
	return id;
}

void Scene::addLight(Light *l){
	this->scene_lights.push_back(l);
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

Animation* Scene::getAnimationByID(string nodeID)
{
	for (unsigned int i = 0; i<animations.size(); i++)
	if (animations[i]->nodeID == nodeID)
		return animations[i];
	throw "Animation not found"; //if animation isn't found, throw error
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
		if (node->animation != NULL && runAnimations)
			node->animation->draw();
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

void Scene::addAnimation(Animation* animation)
{
	this->animations.push_back(animation);
}

void Scene::initAnimations()
{
	for (unsigned int i = 0; i < animations.size(); i++)
		((LinearAnimation*)animations[i])->init();
}

void Scene::setGameMessage(string text)
{
	Interface *interface = (Interface *)iface;
	interface->gameMessage->set_text(text.c_str());
	interface->syncVars();
}

void Scene::placePiece(unsigned int pos)
{
	if (gameState == PLACEPIECE)
	{
		board->previousBoard = board->boardRepresentation; //save previous board
		((Interface *)iface)->undo->disable(); //only undo on the end of each play
		board->boardRepresentation[pos].place(player, pos);
		board->playHistory.push_back({ { pos, 0, 0 , player} });
		gameState = ROTATE;
		checkVictory();
	}
}

void Scene::undoMove()
{
	board->playHistory.pop_back();
	if (gameMode == PVC) //if in PVC, got to clear computer move as well
		board->playHistory.pop_back();

	board->boardRepresentation = board->previousBoard;
	if (gameMode == PVP)
		switchPlayer(); //if in PVC, player is always the same
	else
		updateGameMessage();
	gameState = PLACEPIECE;
	((Interface *)iface)->undo->disable();
}

void Scene::replay()
{
	if (!inReplay) //starting now
	{
		std::array<Piece, 36> cleanBoard;
		board->boardRepresentation = cleanBoard;
		replayPos = 0;
		inReplay = true;
		replayWaitToRotate = false;
		((Interface *)iface)->replay->disable(); //disable replay button
	}

	if (replayPos == board->playHistory.size())
	{
		inReplay = false;
		replayPos = 0;
		((Interface *)iface)->replay->enable(); //re-enable replay button
	}
	else
	{
		player = board->playHistory[replayPos].at(3);
		if (!replayWaitToRotate)
		{
			int pos = board->playHistory[replayPos].at(0);
			board->boardRepresentation[pos].place(player, pos);
			replayWaitToRotate = true; //wait for this animation ending to start rotation
			return;
		}
		else
		{
			replayWaitToRotate = false;
			int quadrant = board->playHistory[replayPos].at(1);
			int direction = board->playHistory[replayPos].at(2);
			if (quadrant > 0) //game can end whithout a rotation
				board->rotateQuadrant(socket, quadrant, direction);
			replayPos++;
		}
	}
}

void Scene::rotateQuadrant(int quadrant, int direction)
{
	if (gameState == ROTATE)
	{
		board->playHistory.back().at(1) = quadrant;
		board->playHistory.back().at(2) = direction;
		board->rotateQuadrant(socket, quadrant, direction);
		checkVictory();
		if (gameState != GAMEOVER)
		{
			if (gameMode == PVC)
				computerPlay();
			else
				switchPlayer();
		}
	}
}

void Scene::checkVictory()
{
	string cmdString = "checkVictory(" + board->getBoardList() + ").\n";
	//cout << cmdString << endl;
	socket->envia(cmdString.c_str(), cmdString.length());
	char answer[256];
	socket->recebe(answer);
	//cout << answer << endl;
	string str = "   ";
	if (answer[0] != '0')
	{
		gameState = GAMEOVER;
		((Interface *)iface)->undo->disable(); //game over, can't undo
		((Interface *)iface)->replay->enable();
		if (answer[0] == '3')
			str += "Draw!";
		else
		{
			str += answer[0]=='1' ? playerOneName : playerTwoName;
			str += " has won!";
		}
		setGameMessage(str);
	}
}

void Scene::computerPlay()
{
	if (!computerPlaying)
	{
		player = !player;
		updateGameMessage();
		board->computerPlacePiece(socket);
		checkVictory();
		if (gameState == GAMEOVER) //if someone won, dont do anything else
			return;
		computerPlaying = true; //wait for animation to end to do rotation
		return;
	}
	else
	{
		board->computerRotateQuadrant(socket);
		computerPlaying = false;
		checkVictory();
		if (gameState != GAMEOVER)
			switchPlayer();
	}
}

void Scene::switchPlayer()
{
	player = !player; //muda para o outro jogador
	updateGameMessage();
	gameState = PLACEPIECE;
	((Interface *)iface)->undo->enable();
}

void Scene::updateGameMessage()
{
	string str = "   ";
	str += player ? playerTwoName : playerOneName;
	str += ", it's your turn!";
	if (gameMode == PVC && player == PLAYERTWO)
		str = "   Computer is playing";
	setGameMessage(str);
}

void Scene::changeGameEnvironment(int gameEnvironment)
{
	this->gameEnvironment = gameEnvironment;
	if (gameEnvironment == 1)
	{
		playerOneName = "White";
		playerTwoName = "Black";
		Piece::playerOnePiece = Piece::white;
		Piece::playerTwoPiece = Piece::black;
		nodes["tabuleiro"]->appearance = appearances[0];
	}
	else if (gameEnvironment == 2)
	{
		playerOneName = "Red";
		playerTwoName = "White";
		Piece::playerOnePiece = Piece::red;
		Piece::playerTwoPiece = Piece::white;
		nodes["tabuleiro"]->appearance = appearances[1];
	}

	if (gameStarted)
	{
		updateGameMessage();
		checkVictory();
	}
}

void Scene::startNewGame()
{
	std::array<Piece, 36> cleanBoard;
	board->boardRepresentation = cleanBoard;
	board->playHistory.clear(); //clear history
	Animation::animationRunning = false; //clear lock state
	this->gameStarted = true;
	this->player = PLAYERONE;
	this->gameState = PLACEPIECE;
}