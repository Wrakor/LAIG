#include "Interface.h"

void Interface::initGUI() {

	gameTypePanel = addPanel("Game Type", GLUI_PANEL_EMBOSSED);
	
	listBox = addListboxToPanel(gameTypePanel, "", 0, 1);
	listBox->add_item(0, "Player vs Player");
	listBox->add_item(1, "Player vs Computer");
	listBox->set_int_val(0);	

	addColumn();

	gameDifficultyPanel = addPanel("Game Difficulty", GLUI_PANEL_EMBOSSED);

	listBox2 = addListboxToPanel(gameDifficultyPanel, "", 0, 2);
	listBox2->add_item(0, "Normal");
	listBox2->add_item(1, "Hard");
	listBox2->set_int_val(0);

	addColumn();

	gameStartPanel = addPanel("Game", GLUI_PANEL_EMBOSSED);
	start = addButtonToPanel(gameStartPanel, "Start Game", 3);	
	//start->set_alignment(GLUI_ALIGN_RIGHT);
	addColumn();
	GLUI_Panel *gameEnvironment = addPanel("Game Environment", GLUI_PANEL_EMBOSSED);
	GLUI_Listbox *gameEnvironmentL = addListboxToPanel(gameEnvironment, "", 0, 4);
	gameEnvironmentL->add_item(0, "Environment 1");
	gameEnvironmentL->add_item(1, "Environment 2");

	addColumn();
	/*GLUI_Panel* quadrantPanel1 = addPanel("main", GLUI_PANEL_NONE);
	GLUI_Button* rotateLeft1 = addButtonToPanel(quadrantPanel1, "<-(1)", 90);
	GLUI_Button* rotateRight1 = addButtonToPanel(quadrantPanel1, "->(1)", 91);
	addColumn();
	GLUI_Panel* quadrantPanel2 = addPanel("main", GLUI_PANEL_NONE);
	GLUI_Button* rotateLeft2 = addButtonToPanel(quadrantPanel2, "<-(2)", 92);
	GLUI_Button* rotateRight2 = addButtonToPanel(quadrantPanel2, "->(2)", 93);
	addColumn();
	GLUI_Panel* quadrantPanel3 = addPanel("main", GLUI_PANEL_NONE);
	GLUI_Button* rotateLeft3 = addButtonToPanel(quadrantPanel3, "<-(3)", 94);
	GLUI_Button* rotateRight3 = addButtonToPanel(quadrantPanel3, "->(3)", 95);
	addColumn();
	GLUI_Panel* quadrantPanel4 = addPanel("main", GLUI_PANEL_NONE);
	GLUI_Button* rotateLeft4 = addButtonToPanel(quadrantPanel4, "<-(4)", 96);
	GLUI_Button* rotateRight4 = addButtonToPanel(quadrantPanel4, "->(4)", 97);
	addColumn();*/
	GLUI_Panel* mainPanel = addPanel("main", GLUI_PANEL_NONE);
	string playerName = "Jogador " + scene->playerOneName;
	char tmpPlayerName[100];
	strcpy(tmpPlayerName, playerName.c_str());
	gameMessage = addStaticTextToPanel(mainPanel, tmpPlayerName);
	GLUI_Button* exit = addButtonToPanel(mainPanel, "Exit Game", 99);
}


void Interface::initGUI2()
{
	gameTypePanel->disable();
	gameDifficultyPanel->disable();
	gameStartPanel->disable();
	listBox->disable();
	listBox2->disable();
	start->disable();
	//addSeparator();
	addColumn();
	///////////////////////////////// Cameras ///////////////////////////////// 

	GLUI_Panel* cameraPanel = addPanel("Cameras", GLUI_PANEL_EMBOSSED);
	cameraListbox = addListboxToPanel(cameraPanel, "", 0, 0);
	addColumn();

	for (unsigned int i = 0; i < scene->getNumCameras(); i++)
	{
		Camera *thisCamera = (Camera *)scene->getCamera(i);

		cameraListbox->add_item(i, thisCamera->nodeID.c_str());

		if (thisCamera == scene->getActiveCamera())
			//cameraRadioGroup->set_int_val(i);
			cameraListbox->set_int_val(i);
	}
}

void Interface::processGUI(GLUI_Control *ctrl)
{
	if (ctrl->user_id == 0)
	{
		Camera *activeCamera = scene->getActiveCamera();
		scene->getCamera(ctrl->get_int_val())->createCameraAnimation(activeCamera->getPosition());
		scene->activateCamera(ctrl->get_int_val());	//switch camera
	}
	else if (ctrl->user_id == 3)
	{
		initGUI2();
	}
	else if (ctrl->user_id == 4)
	{
		if (ctrl->get_int_val() == 1)
		{
			scene->changeGameEnvironment(2);
		}
		else
		{
			scene->changeGameEnvironment(1);
		}
	}
	/*else if (ctrl->user_id == 90)
		scene->rotateQuadrant(1, LEFT);
	else if (ctrl->user_id == 91)
		scene->rotateQuadrant(1, RIGHT);
	else if (ctrl->user_id == 92)
		scene->rotateQuadrant(2, LEFT);
	else if (ctrl->user_id == 93)
		scene->rotateQuadrant(2, RIGHT);
	else if (ctrl->user_id == 94)
		scene->rotateQuadrant(3, LEFT);
	else if (ctrl->user_id == 95)
		scene->rotateQuadrant(3, RIGHT);
	else if (ctrl->user_id == 96)
		scene->rotateQuadrant(4, LEFT);
	else if (ctrl->user_id == 97)
		scene->rotateQuadrant(4, RIGHT);*/
	else if (ctrl->user_id == 99)
	{
		throw("exit"); //exit normally
	}
}

void Interface::setScene(Scene *scene)
{
	this->scene = scene;
}

// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

void Interface::processMouse(int button, int state, int x, int y)
{
	CGFinterface::processMouse(button, state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x, y);
}

void Interface::performPicking(int x, int y)
{
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for 
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix((GLdouble)x, (GLdouble)(CGFapplication::height - y), 5.0, 5.0, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	scene->display();

	// restore original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void Interface::processHits(GLint hits, GLuint buffer[])
{
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected = NULL;
	GLuint nselected;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i = 0; i<hits; i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected = num;
		}
		for (int j = 0; j < num; j++)
			ptr++;
	}

	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected != NULL)
	{
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
		for (unsigned int i = 0; i<nselected; i++)
			printf("%d ", selected[i]);
		printf("\n");
		if (selected[0]<100)
			scene->placePiece(selected[0]);
		else
		{
			switch (selected[0])
			{
			case 100:
				scene->rotateQuadrant(1, LEFT);
				break;
			case 101:
				scene->rotateQuadrant(1, RIGHT);
				break;
			case 102:
				scene->rotateQuadrant(2, LEFT);
				break;
			case 103:
				scene->rotateQuadrant(2, RIGHT);
				break;
			case 104:
				scene->rotateQuadrant(3, LEFT);
				break;
			case 105:
				scene->rotateQuadrant(3, RIGHT);
				break;
			case 106:
				scene->rotateQuadrant(4, LEFT);
				break;
			case 107:
				scene->rotateQuadrant(4, RIGHT);
				break;
			}
		}
	}
	//else
		//printf("Nothing selected while picking \n");
}


void startInterface::initGUI()
{
	GLUI_Panel* gameTypePanel = addPanel("Game Type", GLUI_PANEL_EMBOSSED);
	
	GLUI_Listbox *listBox = addListboxToPanel(gameTypePanel, "", 0, 0);
	listBox->add_item(0, "Player vs Player");
	listBox->add_item(1, "Player vs Computer");
	listBox->set_int_val(0);

	addColumn();

	GLUI_Panel* gameDifficultyPanel = addPanel("Game Difficulty", GLUI_PANEL_EMBOSSED);

	GLUI_Listbox *listBox2 = addListboxToPanel(gameDifficultyPanel, "", 0, 1);
	listBox2->add_item(0, "Normal");
	listBox2->add_item(1, "Hard");
	listBox2->set_int_val(0);
	
	addColumn();

	GLUI_Panel* gameStartPanel = addPanel("Game", GLUI_PANEL_EMBOSSED);
	GLUI_Button *start = addButtonToPanel(gameStartPanel, "Start Game", 2);
	//start->set_alignment(GLUI_ALIGN_RIGHT);
}

void startInterface::processGUI(GLUI_Control *ctrl)
{
	if (ctrl->user_id == 2)
	{
		//initGUI2();
	}

	cout << ctrl->user_id;	
}

void startInterface::setScene(Scene *scene)
{
	this->scene = scene;
}