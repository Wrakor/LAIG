#include "Node.h"

Node::Node(string id){
	this->id=id;
	this->appearance=NULL;
	this->parent=NULL;
}

Appearance* Node::getAppearance()
{
	if(this->appearance)
		return this->appearance;
	else
	{
		if(parent)
			return parent->getAppearance();
		else
			throw "Couldn't find any appereance ref in this branch"; //n�o h� qualquer apar�ncia a aplicar, nem no n� nem na sua linhagem
	}
}