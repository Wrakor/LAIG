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
