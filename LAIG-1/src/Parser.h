#ifndef _PARSER_H_
#define _PARSER_H_

#include "tinyxml.h"
#include "DemoScene.h"
#include <stdexcept>

class Parser
{
public:
	Parser(char *filename);
	~Parser();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	DemoScene scene;
	
protected:

	TiXmlDocument* doc;
	TiXmlElement* yafElement;
	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* lightingElement;
	TiXmlElement* texturesElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* graphElement;
	void parseGlobals();
	void parseCameras();
	void parseLighting();
	void parseTextures();
	void parseAppearances();
	void parseGraph();
};

bool to_bool(std::string str);

#endif