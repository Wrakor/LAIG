#include "Interface.h"

void Interface::initGUI()
{

	///////////////////////////////// Cameras ///////////////////////////////// 
	GLUI_Panel* cameraPanel = addPanel("Cameras", GLUI_PANEL_EMBOSSED);
	//GLUI_RadioGroup *cameraRadioGroup = addRadioGroupToPanel(cameraPanel, 0, 0);
	cameraListbox = addListboxToPanel(cameraPanel, "", 0, 0);
	addColumn();

	for(unsigned int i=0;i<scene->getNumCameras();i++)
	{
		Camera *thisCamera = (Camera *)scene->getCamera(i);

		//std::strcpy(name, thisCamera->nodeID.c_str());
		//addRadioButtonToGroup(cameraRadioGroup, name);
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

		char *name = new char[thisLight->nodeID.size()+1]; //define char pointer com tamanho da string nodeID (addCheckboxToPanel não aceita const char)
		std::strcpy(name, thisLight->nodeID.c_str());	

		GLUI_Checkbox * light = addCheckboxToPanel(lightPanel, name, NULL, GL_LIGHT0+i);
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
}

void Interface::setScene(Scene *scene)
{
	this->scene = scene;
}
