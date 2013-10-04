#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include "Parser.h"

using namespace std;


/*Recebe:
* string text - com varios numeros,
* int n - o numero de numeros a tirar da string,
* vector elements - onde sao guardados os numeros */ 
void extractElementsFromString(vector<float> &elements, string text, int n)
{
	stringstream text_ss;
	text_ss << text;

	for (int i = 0; i < n; i++)
	{
		float j;
		text_ss >> j;
		elements.push_back(j);
	}
}

Parser::Parser(char *filename)
{

	// Read XML from file

	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc() );
		exit( 1 );
	}

	TiXmlElement* yafElement= doc->FirstChildElement( "yaf" );

	if (yafElement == NULL)
	{
		printf("Main yaf block element not found! Exiting!\n");
		exit(1);
	}

	/////////////////////////////////////////////////////////////// Globals ///////////////////////////////////////////////////////////////

	globalsElement = yafElement->FirstChildElement( "globals" );
	if(!globalsElement)
		throw "Error parsing globals";

	string background, drawmode, shading, cullface, cullorder;

	background = globalsElement->Attribute("background");
	drawmode = globalsElement->Attribute("drawmode");
	shading = globalsElement->Attribute("shading");
	cullface = globalsElement->Attribute("cullface");
	cullorder = globalsElement->Attribute("cullorder");

	if(background.empty() || drawmode.empty() || shading.empty() || cullface.empty() || cullorder.empty())
		throw "Error parsing globals attributes";

	//print globals attributes
	cout << "Globals" << endl;
	cout << "\tBackground: " << background << endl;
	cout << "\tDrawmode: " << drawmode << endl;
	cout << "\tShading: " << shading << endl;
	cout << "\tCullface: " << cullface << endl;
	cout << "\tCullorder: " << cullorder << endl;


	/////////////////////////////////////////////////////////////// Cameras ///////////////////////////////////////////////////////////////

	camerasElement = yafElement->FirstChildElement( "cameras" );
	if(!camerasElement)
		throw "Error parsing cameras";

	string initialCamera = camerasElement->Attribute("initial");

	if(initialCamera.empty())
		throw "Error parsing initial camera";

	TiXmlElement* camera = findChildByAttribute(camerasElement, "id", initialCamera.c_str());

	if(!camera)
		throw "Initial camera not declared";
	cout << "Cameras" << endl;
	cout << "\tInitial: " << initialCamera << endl;
	camera = camerasElement->FirstChildElement();

	while(camera)
	{
		float near, far, left, right, top, bottom, angle, pos_x, pos_y, pos_z, target;

		string id = camera->Attribute("id"),type = camera->Value(), pos;		
		vector<float> pos_vector;// = {0,0,0};

		camera->QueryFloatAttribute("near", &near);
		camera->QueryFloatAttribute("far", &far);

		cout << endl << "\tID: " << id << endl;
		cout << "\tType: " << type << endl;
		cout << "\tNear: " << near << endl;
		cout << "\tFar: " << far << endl;

		if (type == "perspective")
		{
			camera->QueryFloatAttribute("angle", &angle);
			pos = camera->Attribute("pos");

			extractElementsFromString(pos_vector, pos, 3);
			camera->QueryFloatAttribute("target", &target);

			//print camera attributes
			cout << "\tAngle: " << angle << endl;
			cout << "\tPos: ";
			
			for (int i = 0; i < pos_vector.size(); i++)
				cout << pos_vector[i] << " ";

			cout << endl;
			cout << "\tTarget: " << target << endl;
			
		}
		else if (type == "ortho")
		{
			camera->QueryFloatAttribute("left", &left);
			camera->QueryFloatAttribute("right", &right);
			camera->QueryFloatAttribute("top", &top);
			camera->QueryFloatAttribute("bottom", &bottom);

			//print camera attributes
			cout << "\tLeft: " << left << endl;
			cout << "\tRight: " << right << endl;
			cout << "\tTop: " << top << endl;
			cout << "\tBottom: " << bottom << endl;
		}


		camera = camera->NextSiblingElement();
	}

	/////////////////////////////////////////////////////////////// Lighting ///////////////////////////////////////////////////////////////

	lightingElement = yafElement->FirstChildElement( "lighting" );
	if(!lightingElement)
		throw "Error parsing lighting";

	bool doublesided, local, enabled;
	vector<float> ambient;

	doublesided = to_bool(lightingElement->Attribute("doublesided"));
	local = to_bool(lightingElement->Attribute("local"));
	enabled = to_bool(lightingElement->Attribute("enabled"));
	extractElementsFromString(ambient, lightingElement->Attribute("ambient"), 4);

	if(ambient.empty())
		throw "Error parsing lighting attributes";

	TiXmlElement *child = lightingElement->FirstChildElement();

	cout << "Lighting" << endl;
	cout << "\tDoublesided: " << boolalpha << doublesided << endl;
	cout << "\tLocal: " << boolalpha << local << endl;
	cout << "\tEnabled: " << boolalpha << enabled << endl;
	cout << "\tAmbient: ";
	for (int i = 0; i < ambient.size(); i++)
		cout << ambient[i] << " ";
	
	string id, type;
	vector<float> location, diffuse, specular, direction;
	ambient.clear(); //limpar o conteudo do vector ambient visto que esta a ser reusado e tinha conteudo adicionado previamente
	float angle, exponent;

	if(!child)
		throw "Error parsing lighting";
	else
		while(child)
	{
		type = child->Value();
		id = child->Attribute("id");
		enabled = to_bool(child->Attribute("enabled"));
		extractElementsFromString(location, child->Attribute("location"), 3);
		extractElementsFromString(ambient, child->Attribute("ambient"), 4);
		extractElementsFromString(diffuse, child->Attribute("diffuse"), 4);
		extractElementsFromString(specular, child->Attribute("specular"), 4);

		cout << "\n\n\t- ID: " << id << endl;
		cout << "\tType: " << type << endl;
		cout << "\tLocation: ";
		for (int i = 0; i < location.size(); i++)
			cout << location[i] << " ";
		cout << "\n\tAmbient: ";
		for (int i = 0; i < ambient.size(); i++)
			cout << ambient[i] << " ";
		cout << "\n\tDiffuse: ";
		for (int i = 0; i < diffuse.size(); i++)
			cout << diffuse[i] << " ";
		cout << "\n\tSpecular: ";
		for (int i = 0; i < specular.size(); i++)
			cout << specular[i] << " ";



		child = child->NextSiblingElement();
	}

	


	/////////////////////////////////////////////////////////////// Textures ///////////////////////////////////////////////////////////////

	texturesElement = yafElement->FirstChildElement( "textures" );

	if(!texturesElement)
		throw "Error parsing textures";

	string file_name;
	cout << "\nTextures" << endl;

	/////////////////////////////////////////////////////////////// Appearances ////////////////////////////////////////////////////////////

	appearancesElement = yafElement->FirstChildElement( "appearances" );

	if(!appearancesElement)
		throw "Error parsing appearances";

	cout << "Appearances" << endl;

	/////////////////////////////////////////////////////////////// Graph //////////////////////////////////////////////////////////////////

	graphElement = yafElement->FirstChildElement( "graph" );

	if(!graphElement)
		throw "Error parsing graph";

	string rootid;

	rootid = graphElement->Attribute("rootid");

	if(rootid.empty())
		throw "Error parsing graph attributes";

	cout << "Graph" << endl;
	cout << "\tRoot ID" << rootid << endl;
}
/*	// Init
// An example of well-known, required nodes

if (initElement == NULL)
printf("Init block not found!\n");
else
{
printf("Processing init:\n");
// frustum: example of a node with individual attributes
TiXmlElement* frustumElement=initElement->FirstChildElement("frustum");
if (frustumElement)
{
float near,far;

if (frustumElement->QueryFloatAttribute("near",&near)==TIXML_SUCCESS && 
frustumElement->QueryFloatAttribute("far",&far)==TIXML_SUCCESS
)
printf("  frustum attributes: %f %f\n", near, far);
else
printf("Error parsing frustum\n");
}
else
printf("frustum not found\n");


// translate: example of a node with an attribute comprising several float values
// It shows an example of extracting an attribute's value, and then further parsing that value 
// to extract individual values
TiXmlElement* translateElement=initElement->FirstChildElement("translate");
if (translateElement)
{
char *valString=NULL;
float x,y,z;

valString=(char *) translateElement->Attribute("xyz");

if(valString && sscanf(valString,"%f %f %f",&x, &y, &z)==3)
{
printf("  translate values (XYZ): %f %f %f\n", x, y, z);
}
else
printf("Error parsing translate");
}
else
printf("translate not found\n");		

// repeat for each of the variables as needed
}

// Other blocks could be validated/processed here


// graph section
if (graphElement == NULL)
printf("Graph block not found!\n");
else
{
char *prefix="  -";
TiXmlElement *node=graphElement->FirstChildElement();

while (node)
{
printf("Node id '%s' - Descendants:\n",node->Attribute("id"));
TiXmlElement *child=node->FirstChildElement();
while (child)
{
if (strcmp(child->Value(),"Node")==0)
{
// access node data by searching for its id in the nodes section

TiXmlElement *noderef=findChildByAttribute(nodesElement,"id",child->Attribute("id"));

if (noderef)
{
// print id
printf("  - Node id: '%s'\n", child->Attribute("id"));

// prints some of the data
printf("    - Material id: '%s' \n", noderef->FirstChildElement("material")->Attribute("id"));
printf("    - Texture id: '%s' \n", noderef->FirstChildElement("texture")->Attribute("id"));

// repeat for other leaf details
}
else
printf("  - Node id: '%s': NOT FOUND IN THE NODES SECTION\n", child->Attribute("id"));

}
if (strcmp(child->Value(),"Leaf")==0)
{
// access leaf data by searching for its id in the leaves section
TiXmlElement *leaf=findChildByAttribute(leavesElement,"id",child->Attribute("id"));

if (leaf)
{
// it is a leaf and it is present in the leaves section
printf("  - Leaf id: '%s' ; type: '%s'\n", child->Attribute("id"), leaf->Attribute("type"));

// repeat for other leaf details
}
else
printf("  - Leaf id: '%s' - NOT FOUND IN THE LEAVES SECTION\n",child->Attribute("id"));
}

child=child->NextSiblingElement();
}
node=node->NextSiblingElement();
}
}

}*/

Parser::~Parser()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *Parser::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
	// Searches within descendants of a parent for a node that has an attribute _attr_ (e.g. an id) with the value _val_
	// A more elaborate version of this would rely on XPath expressions
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}

bool to_bool(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}