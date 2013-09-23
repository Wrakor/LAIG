#ifndef _PARSER_H_
#define _PARSER_H_

#include "tinyxml.h"
#include <stdexcept>

class Parser
{
public:
	Parser(char *filename);
	~Parser();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	
protected:

	TiXmlDocument* doc;
	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* lightingElement;
	TiXmlElement* texturesElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* graphElement;
};

#endif