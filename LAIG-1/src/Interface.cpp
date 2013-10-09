#include "Interface.h"
#include "OrthoCamera.h"

void Interface::initGUI()
{
	GLUI_Panel* cameraPanel = addPanel("Câmaras", GLUI_PANEL_NONE);
	GLUI_RadioGroup *cameraRadioGroup = addRadioGroupToPanel(cameraPanel, 0, 0);
	addColumn();
	char name[255];

	for(unsigned int i=0;i<scene->getNumCameras();i++)
	{
		std::strcpy(name, ((OrthoCamera *)scene->getCamera(i))->nodeID.c_str());
		addRadioButtonToGroup(cameraRadioGroup, name);
	}
}

void Interface::processGUI(GLUI_Control *ctrl)
{
	if(ctrl->user_id == 0)
		scene->activateCamera(scene->getCameraIDByID(ctrl->name));
}

void Interface::setScene(Scene *scene)
{
	this->scene = scene;
}