#pragma once
#include "CGFinterface.h"
#include "Scene.h"


class Interface : public CGFinterface
{
	Scene *scene;
	GLUI_Listbox *cameraListbox;
public:
	void initGUI();
	void setScene(Scene *scene);
	void processGUI(GLUI_Control *ctrl);
};