#include <string>
#include <vector>
using namespace std;

class Node {
	string id;
	string appearance; //id da appearance
	float T[16]; //matriz de transformações
	vector<string> children; //vector de ids dos filhos
	vector<Primitiva*> primitivas;
};