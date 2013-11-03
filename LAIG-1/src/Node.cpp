#include "Node.h"

Node::Node(string id)
{
	this->id = id;
	this->appearance=NULL;
	this->displayList = false;
	this->displayListID = NULL;
	this->animation = NULL;
}