#pragma once
#include "CGFinterface.h"
#include "Scene.h"

class Interface : public CGFinterface
{
	Scene *scene;
	GLUI_Listbox *cameraListbox;

public:
	void initGUI();
	void startGame();
	void setScene(Scene *scene);
	void processGUI(GLUI_Control *ctrl);
	virtual void processMouse(int button, int state, int x, int y);
	void performPicking(int x, int y);
	void processHits(GLint hits, GLuint buffer[]);
	GLUI_Panel* gameSettingsPanel, *gameStartPanel;
	GLUI_Listbox *listBox, *listBox2;
	GLUI_Button *start, *undo, *replay;
	GLUI_StaticText *gameMessage;
};
