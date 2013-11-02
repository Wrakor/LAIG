#pragma once

#include <string>
#include <vector>
#include "Primitiva.h"
#include "Appearance.h"


using namespace std;

class Node {
public:
	Node(string id);
	string id;
	Appearance *appearance;
	float T[16]; //matriz de transformações
	vector<string> children; //vector de ids dos filhos
	vector<Primitiva*> primitivas;
	bool displayList;
	GLuint displayListID;
};