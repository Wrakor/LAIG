#pragma once
#include "CGFinterface.h"
#include "Scene.h"


class Interface : public CGFinterface
{
	Scene *scene;
public:
	void initGUI();
	void setScene(Scene *scene);
	void processGUI(GLUI_Control *ctrl);
};