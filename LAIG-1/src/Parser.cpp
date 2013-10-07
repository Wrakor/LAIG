#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include "Parser.h"
#include "PerspectiveCamera.h"
#include "OrthoCamera.h"

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

void Parser::parseGlobals()
{
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
}

void Parser::parseCameras()
{
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
		float near, far, left, right, top, bottom, angle, pos_x, pos_y, pos_z;
		int cameraVectorIndex = 0;

		string id = camera->Attribute("id"),type = camera->Value(), pos, target;		
		vector<float> pos_vector;// = {0,0,0};
		vector<float> target_vector;

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

			target = camera->Attribute("target");
			extractElementsFromString(target_vector, target, 3);

			//print camera attributes
			cout << "\tAngle: " << angle << endl;
			cout << "\tPos: ";

			for (int i = 0; i < pos_vector.size(); i++)
				cout << pos_vector[i] << " ";

			cout << endl;

			cout << "\tTarget: ";

			for (int i = 0; i < target_vector.size(); i++)
				cout << target_vector[i] << " ";

			cout << endl;

			PerspectiveCamera *c = new PerspectiveCamera(near, far, angle);
			c->setX(pos_vector[0]);
			c->setY(pos_vector[1]);
			c->setZ(pos_vector[2]);

			c->setTargetX(target_vector[0]);
			c->setTargetY(target_vector[1]);
			c->setTargetZ(target_vector[2]);

			cameraVectorIndex = this->scene.addCamera(c);

			//c->setRotation(CG_CGFcamera_AXIS_Y, angle); Qual ângulo?
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

			OrthoCamera *c = new OrthoCamera(near, far, left, right, top, bottom);

			cameraVectorIndex = this->scene.addCamera(c);
		}

		if(camera == initialCamera)
			this->scene.activateCamera(cameraVectorIndex);

		camera = camera->NextSiblingElement();
	}
}

void Parser::parseLighting()
{
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
}

void Parser::parseTextures()
{
	string id;
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
}

void Parser::parseAppearances()
{
	appearancesElement = yafElement->FirstChildElement( "appearances" );
	string id;
	vector<float> ambient, diffuse, specular;
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
}

void Parser::parseGraph()
{

	graphElement = yafElement->FirstChildElement( "graph" );

	if(!graphElement)
		throw "Error parsing graph";

	string rootid, id;

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
			TiXmlElement *transformsElement = transforms->FirstChildElement();

			cout << "\t-Transforms" << endl;

			if (!transforms)
				throw "Error parsing transforms";
			else
				while (transformsElement)
				{						
					vector<float> transforms_components, scale;
					float angle;
					string axis, value = transformsElement->Value();

					if (value  == "translate")
					{
						extractElementsFromString(transforms_components, transformsElement->Attribute("to"), 3);

						cout << "\tTranslate to: ";
						for (int i = 0; i < transforms_components.size(); i++)						
							cout << transforms_components[i] << " ";
						cout << endl;

						transforms_components.clear();
					}
					else if (value == "rotate")
					{
						axis = transformsElement->Attribute("axis");

						if (transformsElement->QueryFloatAttribute("angle", &angle) == 0)
						{}

						cout << "\tRotate axis " << axis << " by " << angle <<" degrees" << endl;
					}
					else if (value == "scale")
					{
						extractElementsFromString(transforms_components, transformsElement->Attribute("factor"), 3);

						cout << "\tScale factor: ";
						for (int i = 0; i < transforms_components.size(); i++)
						{
							cout << transforms_components[i] << " ";
						}
						cout << endl;
					}

					transformsElement = transformsElement->NextSiblingElement();
				}

				string appearanceref, value;
				TiXmlElement * children = transforms->NextSiblingElement(); //pode ser children ou appearanceref
				value = children->Value();

				if (value == "appearanceref")
				{
					appearanceref = children->Attribute("id");
					cout << "\t-Appearanceref ID: " << appearanceref << endl;
				}
				else if (value == "children")
				{
					TiXmlElement *childrenElement = children->FirstChildElement();

					cout << "\t-Children:" << endl;
					while (childrenElement)
					{
						value = childrenElement->Value();

						if (value == "rectangle")
						{
							vector<float> xy1, xy2;

							extractElementsFromString(xy1, childrenElement->Attribute("xy1"), 2);
							extractElementsFromString(xy2, childrenElement->Attribute("xy2"), 2);

							cout << "\tRectangle: ";
							for (int i = 0; i < xy1.size(); i++)
								cout << xy1[i] << " ";
							cout << endl;

						}
						else if (value == "triangle")
						{
							vector<float> xyz1, xyz2, xyz3;

							extractElementsFromString(xyz1, childrenElement->Attribute("xyz1"),	3);
							extractElementsFromString(xyz2, childrenElement->Attribute("xyz2"),	3);
							extractElementsFromString(xyz3, childrenElement->Attribute("xyz3"),	3);
						}
						else if (value == "cylinder")
						{
							float base, top, height;
							int slices, stacks;

							childrenElement->QueryFloatAttribute("base", &base);
							childrenElement->QueryFloatAttribute("top", &top);
							childrenElement->QueryFloatAttribute("height", &height);
							childrenElement->QueryIntAttribute("slices", &slices);
							childrenElement->QueryIntAttribute("stacks", &stacks);
						}
						else if (value == "sphere")
						{
							float radius;
							int slices, stacks;

							childrenElement->QueryFloatAttribute("radius", &radius);
							childrenElement->QueryIntAttribute("slices", &slices);
							childrenElement->QueryIntAttribute("stacks", &stacks);
						}
						else if (value == "torus")
						{
							float inner, outer;
							int slices, loops;

							childrenElement->QueryFloatAttribute("inner", &inner);
							childrenElement->QueryFloatAttribute("outer", &outer);
							childrenElement->QueryIntAttribute("slices", &slices);
							childrenElement->QueryIntAttribute("loops", &loops);
						}
						else if (value == "noderef")
						{
							int id;

							childrenElement->QueryIntAttribute("id", &id);
						}

						childrenElement = childrenElement->NextSiblingElement();
					}
				}			
				node = node->NextSiblingElement();
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

	yafElement = doc->FirstChildElement( "yaf" );


	if (yafElement == NULL)
	{
		printf("Main yaf block element not found! Exiting!\n");
		exit(1);
	}

	parseGlobals();
	parseCameras();
	parseLighting();
	parseTextures();	
	parseAppearances();
	parseGraph();
}

Parser::~Parser()
{
	delete(doc);
}

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