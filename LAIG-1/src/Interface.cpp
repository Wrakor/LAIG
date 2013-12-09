#include "Interface.h"

void Interface::initGUI()
{

	///////////////////////////////// Cameras ///////////////////////////////// 
	GLUI_Panel* cameraPanel = addPanel("Cameras", GLUI_PANEL_EMBOSSED);
	cameraListbox = addListboxToPanel(cameraPanel, "", 0, 0);
	addColumn();

	for(unsigned int i=0;i<scene->getNumCameras();i++)
	{
		Camera *thisCamera = (Camera *)scene->getCamera(i);

		cameraListbox->add_item(i, thisCamera->nodeID.c_str());

		if (thisCamera == scene->getActiveCamera())
			//cameraRadioGroup->set_int_val(i);
			cameraListbox->set_int_val(i);
	}

	addButtonToPanel(cameraPanel, "Reset", 1);

	///////////////////////////////// Luzes ///////////////////////////////// 
	GLUI_Panel* lightPanel = addPanel("Lights", GLUI_PANEL_EMBOSSED);

	for (unsigned int i = 0; i < scene->getNumLights(); i++)
	{
		Light *thisLight = scene->getLightByGLFloat(GL_LIGHT0+i);

		GLUI_Checkbox * light = addCheckboxToPanel(lightPanel, (char *)thisLight->nodeID.c_str(), NULL, GL_LIGHT0+i);
		if (thisLight->isEnabled())		
			light->set_int_val(1);	
	}
	addColumn();

	///////////////////////////////// Polygon Mode /////////////////////////////////	
	GLUI_Panel *polygonModePanel = addPanel("Polygon Mode", GLUI_PANEL_EMBOSSED);
	GLUI_RadioGroup *polygonModeRadioGroup = addRadioGroupToPanel(polygonModePanel, 0, 2);
	addRadioButtonToGroup(polygonModeRadioGroup, "Points");
	addRadioButtonToGroup(polygonModeRadioGroup, "Lines");
	addRadioButtonToGroup(polygonModeRadioGroup, "Fill");

	switch (scene->drawMode)
	{
	case (GL_POINT):
		polygonModeRadioGroup->set_int_val(0);
		break;
	case (GL_LINE):
		polygonModeRadioGroup->set_int_val(1);
		break;
	case (GL_FILL):
		polygonModeRadioGroup->set_int_val(2);
		break;
	}
	addColumn();
	///////////////////////////////// Animations /////////////////////////////////	

	GLUI_Panel* animationsPanel = addPanel("Animations", GLUI_PANEL_NONE);

	GLUI_Checkbox * animations = addCheckboxToPanel(animationsPanel, "Animations", NULL, 3);
	if (scene->runAnimations)
		animations->set_int_val(1);
}

void Interface::processGUI(GLUI_Control *ctrl)
{
	if(ctrl->user_id == 0)
		scene->activateCamera(ctrl->get_int_val());	
	else if (ctrl->user_id >= GL_LIGHT0 && ctrl->user_id <= GL_LIGHT7)
	{
		Light *thisLight = scene->getLightByGLFloat(ctrl->user_id);

		if (ctrl->get_int_val() == GL_TRUE)
			thisLight->enable();
		else
			thisLight->disable();		
	}
	else if (ctrl->user_id == 1)
	{
		scene->getCamera(cameraListbox->get_int_val())->reset();
	}
	else if (ctrl->user_id == 2)
	{		
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT+ctrl->get_int_val()); //GL_POINT < GL_LINE < GL_FILL
		scene->setDrawMode(GL_POINT+ctrl->get_int_val());
	}
	else if (ctrl->user_id == 3)
	{
		scene->runAnimations = !scene->runAnimations;
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
		for (int i = 0; i<nselected; i++)
			printf("%d ", selected[i]);
		printf("\n");
		scene->placePiece(selected[0]);
	}
	else
		printf("Nothing selected while picking \n");
}