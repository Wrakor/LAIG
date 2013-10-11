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
		std::strcpy(name, ((Camera *)scene->getCamera(i))->nodeID.c_str());
		addRadioButtonToGroup(cameraRadioGroup, name);

		if (((Camera *)scene->getCamera(i)) == scene->getActiveCamera())
			cameraRadioGroup->set_int_val(i);
	}

	///////////////////////////////// Luzes ///////////////////////////////// 
	GLUI_Panel* lightPanel = addPanel("Lights", GLUI_PANEL_EMBOSSED);

	for (unsigned int i = 0; i < scene->getNumLights(); i++)
	{
		string a = std::to_string(i);
		std::strcpy(name, "Luz ");
		std::strcat(name, a.c_str());		

		GLUI_Checkbox * light = addCheckboxToPanel(lightPanel, name, NULL, GL_LIGHT0+i);
		if (scene->getLightByGLFloat(GL_LIGHT0+i)->isEnabled())
		{			
			light->set_int_val(1);
		}		
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
		if (ctrl->get_int_val() == GL_TRUE)
			scene->getLightByGLFloat(ctrl->user_id)->enable();
		else
			scene->getLightByGLFloat(ctrl->user_id)->disable();		
	}
	else if (ctrl->user_id == 1)
	{		
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT+ctrl->get_int_val()); //GL_POINT < GL_LINE < GL_FILL
		//scene->drawMode =  GL_POINT+ctrl->get_int_val();
		scene->setDrawMode(GL_POINT+ctrl->get_int_val());
	}
}

void Interface::setScene(Scene *scene)
{
	this->scene = scene;
}