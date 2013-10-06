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

	//INIT Scene
	this->scene = * new DemoScene();

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

	if(drawmode == "fill")
		this->scene.drawMode = GL_FILL;
	else if(drawmode == "line")
		this->scene.drawMode = GL_LINE;
	else if(drawmode == "point")
		this->scene.drawMode = GL_POINT;
	else
		throw "Invalid drawmode";

	if(shading == "flat")
		this->scene.shadeModel = GL_FLAT;
	else if(shading == "gouraud")
		this->scene.shadeModel = GL_SMOOTH;
	else
		throw "Invalid shading";

	if(cullface == "none")
		this->scene.cullface = false;
	else {
		this->scene.cullface = true;
		if(cullface=="back")
			this->scene.cullfaceMode = GL_BACK;
		else if(cullface=="front")
			this->scene.cullfaceMode = GL_FRONT;
		else if(cullface=="both")
			this->scene.cullfaceMode = GL_FRONT_AND_BACK;
		else
			throw "Invalid cullface";
	}

	if(cullorder == "CCW")
		this->scene.cullorder = GL_CCW;
	else if(cullorder == "CW")
		this->scene.cullorder = GL_CW;
	else
		throw "Invalid cullorder";

	/////////////////////////////////////////////////////////////// Cameras ///////////////////////////////////////////////////////////////

	camerasElement = yafElement->FirstChildElement( "cameras" );
	if(!camerasElement)
		throw "Error parsing cameras";

	string initialCameraID = camerasElement->Attribute("initial");
	if(initialCameraID.empty())
		throw "Error parsing initial camera";
	TiXmlElement* initialCamera = findChildByAttribute(camerasElement, "id", initialCameraID.c_str());

	if(!initialCamera)
		throw "Initial camera not declared";
	cout << "Cameras" << endl;



	cout << "\t Initial: " << initialCameraID << endl;
	TiXmlElement* camera = camerasElement->FirstChildElement();


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

	TiXmlElement *lighting = lightingElement->FirstChildElement();

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

	if(!lighting)
		throw "Error parsing lighting";
	else
		while(lighting)
		{
			type = lighting->Value();
			id = lighting->Attribute("id");
			enabled = to_bool(lighting->Attribute("enabled"));
			extractElementsFromString(location, lighting->Attribute("location"), 3);
			extractElementsFromString(ambient, lighting->Attribute("ambient"), 4);
			extractElementsFromString(diffuse, lighting->Attribute("diffuse"), 4);
			extractElementsFromString(specular, lighting->Attribute("specular"), 4);

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

			if (type == "spot")
			{
				lighting->QueryFloatAttribute("angle", &angle);
				lighting->QueryFloatAttribute("exponent", &exponent);
				extractElementsFromString(direction, lighting->Attribute("direction"), 3);

				cout << "\tangle:" << angle << endl;
				cout << "\texponent:" << exponent << endl;
				for (int i = 0; i < direction.size(); i++)
					cout << "\tdirection:" << direction[i] << endl;
			}

			lighting = lighting->NextSiblingElement();
		}




		/////////////////////////////////////////////////////////////// Textures ///////////////////////////////////////////////////////////////

		texturesElement = yafElement->FirstChildElement( "textures" );

		if(!texturesElement)
			throw "Error parsing textures";

		string file_name;
		cout << "\nTextures" << endl;

		TiXmlElement *textures = texturesElement->FirstChildElement("texture");


		while (textures)
		{
			id = textures->Attribute("id");
			file_name = textures->Attribute("file");

			cout << "\tID: " << id << endl;
			cout << "\tFile: " << file_name << endl << endl;

			textures = textures->NextSiblingElement();
		}


		/////////////////////////////////////////////////////////////// Appearances ////////////////////////////////////////////////////////////

		appearancesElement = yafElement->FirstChildElement( "appearances" );

		if(!appearancesElement)
			throw "Error parsing appearances";



		cout << "Appearances" << endl;

		TiXmlElement *appearances = appearancesElement->FirstChildElement("appearance");

		if (!appearances)
			throw "Error parsing appearances";
		while (appearances)
		{
			vector<float> emissive;
			float shininess,  texlength_s,  texlength_t;
			string textureref;
			ambient.clear();
			diffuse.clear();
			specular.clear();

			id = appearances->Attribute("id");
			extractElementsFromString(emissive, appearances->Attribute("emissive"), 4);
			extractElementsFromString(ambient, appearances->Attribute("ambient"), 4);
			extractElementsFromString(diffuse, appearances->Attribute("diffuse"), 4);
			extractElementsFromString(specular, appearances->Attribute("specular"), 4);
			appearances->QueryFloatAttribute("shininess", &shininess);

			cout << "\n\t- ID: " << id << endl;
			cout << "\temissive: ";
			for (int i = 0; i < emissive.size(); i++)
				cout << emissive[i] << " ";
			cout << "\n\tambient: ";
			for (int i = 0; i < ambient.size(); i++)
				cout << ambient[i] << " ";
			cout << "\n\tdiffuse: ";
			for (int i = 0; i < diffuse.size(); i++)
				cout << diffuse[i] << " ";
			cout << "\n\tspecular: ";
			for (int i = 0; i < specular.size(); i++)
				cout << specular[i] << " ";
			cout << "\n\tshininess: " << shininess;

			//textureref, textlength_s e textlength_t sao opcionais   ||NAO APAGAR OS IF's||
			if (appearances->Attribute("textureref") != NULL) 
			{
				textureref = appearances->Attribute("textureref");		
				cout << "\n\ttextureref: " << textureref;
			}
			if (appearances->QueryFloatAttribute("texlength_s", &texlength_s) == 0)
			{
				cout << "\n\ttexlength_s: " << texlength_s;
			}
			if (appearances->QueryFloatAttribute("texlength_t", &texlength_t) == 0)
			{
				cout << "\n\ttexlength_t: " << texlength_t;
			}

			appearances = appearances->NextSiblingElement();
		}



		/////////////////////////////////////////////////////////////// Graph //////////////////////////////////////////////////////////////////

		graphElement = yafElement->FirstChildElement( "graph" );

		if(!graphElement)
			throw "Error parsing graph";

		string rootid;

		rootid = graphElement->Attribute("rootid");

		if(rootid.empty())
			throw "Error parsing graph attributes";

		cout << "\nGraph" << endl;
		cout << "\tRoot ID: " << rootid << endl;

		TiXmlElement *node = graphElement->FirstChildElement("node");

		if (!node)
			throw "Error parsing nodes!";
		else
			while (node)
			{
				id = node->Attribute("id");
				cout << "\n\t-ID: " << id << endl;

				TiXmlElement *transforms = node->FirstChildElement("transforms");
				TiXmlElement *translate = transforms->FirstChildElement("translate");
				TiXmlElement *rotate = transforms->FirstChildElement("rotate");
				TiXmlElement *transformsElement = transforms->FirstChildElement();

				/*if (!transforms)
				throw "Error parsing transforms";
				else
				while (transforms)
				{
				vector<float> transforms_components, scale;
				float angle;
				string axis;

				while (translate)
				{
				extractElementsFromString(transforms_components, translate->Attribute("to"), 3);

				cout << "\tTranslate to: ";
				for (int i = 0; i < transforms_components.size(); i++)						
				cout << transforms_components[i] << " ";
				cout << endl;

				transforms_components.clear();
				translate = translate->NextSiblingElement("translate");
				}

				while (rotate)
				{
				axis = rotate->Attribute("axis");

				if (rotate->QueryFloatAttribute("angle", &angle) == 0)
				{
				cout << "pilas";
				}

				cout << "\tRotate axis " << axis << " by " << angle <<" degrees" << endl;


				rotate = rotate->NextSiblingElement("rotate");
				}

				transforms = transforms->NextSiblingElement();
				}*/

				if (!transforms)
					throw "Error parsing transforms";
				else
					while (transformsElement)
					{						
						vector<float> transforms_components, scale;
						float angle;
						string axis;
						string value = transformsElement->Value();

						if (value  == "translate")
						{
							extractElementsFromString(transforms_components, translate->Attribute("to"), 3);

							cout << "\tTranslate to: ";
							for (int i = 0; i < transforms_components.size(); i++)						
								cout << transforms_components[i] << " ";
							cout << endl;

							transforms_components.clear();
						}
						else if (value == "rotate")
						{
							axis = rotate->Attribute("axis");

							if (rotate->QueryFloatAttribute("angle", &angle) == 0)
							{}

							cout << "\tRotate axis " << axis << " by " << angle <<" degrees" << endl;
						}


						transformsElement = transformsElement->NextSiblingElement();
					}

					node = node->NextSiblingElement();
			}
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