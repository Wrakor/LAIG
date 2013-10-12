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
	float T[16]; //matriz de transforma��es
	vector<string> children; //vector de ids dos filhos
	vector<Primitiva*> primitivas;
	Node *parent;
	Appearance* getAppearance(); //retorna a appearance se existir, se n�o retorna a do pai
};