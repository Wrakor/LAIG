#ifndef _PARSER_H_
#define _PARSER_H_

#include "tinyxml.h"

class Parser
{
public:
	Parser(char *filename);
	~Parser();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	
protected:

	TiXmlDocument* doc;
	TiXmlElement* globalsElement;
};

#endif