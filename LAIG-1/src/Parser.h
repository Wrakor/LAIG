#pragma once

#include "tinyxml.h"
#include "Light.h"
#include "Scene.h"
#include "Interface.h"
#include <stdexcept>

class Parser
{
public:
	Parser(char *filename);
	~Parser();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	Scene scene;
	Interface interface;
	
private:

	TiXmlDocument* doc;
	TiXmlElement* yafElement;
	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* lightingElement;
	TiXmlElement* texturesElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* animationsElement;
	TiXmlElement* graphElement;
	void parseGlobals();
	void parseCameras();
	void parseLighting();
	void parseTextures();
	void parseAppearances();
	void parseAnimations();
	void parseGraph();
	string path;
	
};

bool to_bool(std::string str);
