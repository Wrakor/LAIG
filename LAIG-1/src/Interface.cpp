#include "Interface.h"

void Interface::initGUI()
{

	///////////////////////////////// Cameras ///////////////////////////////// 
	GLUI_Panel* cameraPanel = addPanel("Cameras", GLUI_PANEL_EMBOSSED);
	GLUI_RadioGroup *cameraRadioGroup = addRadioGroupToPanel(cameraPanel, 0, 0);
	addColumn();
	char name[255];

	for(unsigned int i=0;i<scene->getNumCameras();i++)
	{
		Camera *thisCamera = (Camera *)scene->getCamera(i);

		std::strcpy(name, thisCamera->nodeID.c_str());
		addRadioButtonToGroup(cameraRadioGroup, name);

		if (thisCamera == scene->getActiveCamera())
			cameraRadioGroup->set_int_val(i);
	}

	///////////////////////////////// Luzes ///////////////////////////////// 
	GLUI_Panel* lightPanel = addPanel("Lights", GLUI_PANEL_EMBOSSED);

	for (unsigned int i = 0; i < scene->getNumLights(); i++)
	{
		Light *thisLight = scene->getLightByGLFloat(GL_LIGHT0+i);

		string a = std::to_string(i);
		std::strcpy(name, thisLight->nodeID.c_str());	

		GLUI_Checkbox * light = addCheckboxToPanel(lightPanel, name, NULL, GL_LIGHT0+i);
		if (thisLight->isEnabled())		
			light->set_int_val(1);	
	}
	addColumn();

	///////////////////////////////// Polygon Mode /////////////////////////////////	
	GLUI_Panel *polygonModePanel = addPanel("Polygon Mode", GLUI_PANEL_EMBOSSED);
	GLUI_RadioGroup *polygonModeRadioGroup = addRadioGroupToPanel(polygonModePanel, 0, 1);
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT+ctrl->get_int_val()); //GL_POINT < GL_LINE < GL_FILL
		scene->setDrawMode(GL_POINT+ctrl->get_int_val());
	}
}

void Interface::setScene(Scene *scene)
{
	this->scene = scene;
}