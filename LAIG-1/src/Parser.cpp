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
#include "Appearance.h"

using namespace std;


/*Recebe:
* string text - com varios numeros,
* int n - o numero de numeros a tirar da string,
* vector elements - onde sao guardados os numeros */ 
void extractElementsFromString(vector<float> &elements, string text, int n)
{
	stringstream text_ss;
	text_ss << text;
	elements.clear();	

	for (int i = 0; i < n; i++)
	{
		float j;

		if(text_ss.rdbuf()->in_avail()==0) //verifica antes de ler a stringstream se o numero de caracteres a ler é 0, pois se for, significa que faltam elementos pois vai ser lido o ultimo numero outra vez 
			throw "Error parsing, parameter with invalid number of attributes!";

		text_ss >> j;
		elements.push_back(j);
	}

	if(text_ss.rdbuf()->in_avail()!=0)
		throw "Error parsing, parameter with invalid number of attributes!";
}

void Parser::parseGlobals()
{
	globalsElement = yafElement->FirstChildElement( "globals" );
	if(!globalsElement)
		throw "Error parsing globals";

	string drawmode, shading, cullface, cullorder;
	vector<float> background;

	extractElementsFromString(background, globalsElement->Attribute("background"), 4);
	drawmode = globalsElement->Attribute("drawmode");
	shading = globalsElement->Attribute("shading");
	cullface = globalsElement->Attribute("cullface");
	cullorder = globalsElement->Attribute("cullorder");

	if(background.empty() || drawmode.empty() || shading.empty() || cullface.empty() || cullorder.empty())
		throw "Error parsing globals attributes";

	//print globals attributes
	cout << "Globals" << endl;
	cout << "\tBackground: ";
	for (unsigned int i = 0; i < background.size(); i++)
	{
		cout << background[i] << " ";		
	}
	cout << "\n\tDrawmode: " << drawmode << endl;
	cout << "\tShading: " << shading << endl;
	cout << "\tCullface: " << cullface << endl;
	cout << "\tCullorder: " << cullorder << endl;

	scene.backgroundR = background[0];
	scene.backgroundG = background[1];
	scene.backgroundB = background[2];
	scene.backgroundA = background[3];

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
		float near, far, left, right, top, bottom, angle;
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

			for (unsigned int i = 0; i < pos_vector.size(); i++)
				cout << pos_vector[i] << " ";

			cout << endl;

			cout << "\tTarget: ";

			for (unsigned int i = 0; i < target_vector.size(); i++)
				cout << target_vector[i] << " ";

			cout << endl;

			PerspectiveCamera *c = new PerspectiveCamera(id, near, far, angle);
			c->setX(pos_vector[0]);
			c->setY(pos_vector[1]);
			c->setZ(pos_vector[2]);

			c->setTargetX(target_vector[0]);
			c->setTargetY(target_vector[1]);
			c->setTargetZ(target_vector[2]);

			cameraVectorIndex = this->scene.addCamera(c);
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

			OrthoCamera *c = new OrthoCamera(id, near, far, left, right, top, bottom);

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

	this->scene.lightingDoubleSided = doublesided;
	this->scene.lightingLocal = local;
	this->scene.lightingEnabled = enabled;
	for(unsigned int i=0;i<ambient.size();i++) //set all background_ambient array fields with corresponding vector
		Light::background_ambient[i]=ambient[i];

	cout << "Lighting" << endl;
	cout << "\tDoublesided: " << boolalpha << doublesided << endl;
	cout << "\tLocal: " << boolalpha << local << endl;
	cout << "\tEnabled: " << boolalpha << enabled << endl;
	cout << "\tAmbient: ";
	for (unsigned int i = 0; i < ambient.size(); i++)
		cout << ambient[i] << " ";

	TiXmlElement *lighting = lightingElement->FirstChildElement();

	string id, type;
	vector<float> location, diffuse, specular, direction;
	float angle, exponent;
	unsigned int j=0;
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

			location.push_back(1);

			cout << "\n\n\t- ID: " << id << endl;
			cout << "\tType: " << type << endl;
			cout << "\tEnabled: " << boolalpha << enabled << endl;
			cout << "\tLocation: ";
			for (unsigned int i = 0; i < location.size(); i++)
				cout << location[i] << " ";
			cout << "\n\tAmbient: ";
			for (unsigned int i = 0; i < ambient.size(); i++)
				cout << ambient[i] << " ";
			cout << "\n\tDiffuse: ";
			for (unsigned int i = 0; i < diffuse.size(); i++)
				cout << diffuse[i] << " ";
			cout << "\n\tSpecular: ";
			for (unsigned int i = 0; i < specular.size(); i++)
				cout << specular[i] << " ";

			Light *l = new Light(id, false, GL_LIGHT0+j, &location[0]);

			if (type == "spot")
			{
				lighting->QueryFloatAttribute("angle", &angle);
				lighting->QueryFloatAttribute("exponent", &exponent);
				extractElementsFromString(direction, lighting->Attribute("direction"), 3);

				cout << "\tangle:" << angle << endl;
				cout << "\texponent:" << exponent << endl;
				for (unsigned int i = 0; i < direction.size(); i++)
					cout << "\tdirection:" << direction[i] << endl;

				//got to create again to set direction and exponent
				l = new Light(id, true, GL_LIGHT0+j, &location[0], &direction[0], exponent);
				l->setAngle(angle);
			}
			l->setAmbient(&ambient[0]);
			l->setDiffuse(&diffuse[0]);
			l->setSpecular(&specular[0]);
			if(enabled)
				l->enable();
			else
				l->disable();
			this->scene.addLight(l);
			j++;
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

		this->scene.addTexture(new Texture(id, file_name));

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

		id = appearances->Attribute("id");
		extractElementsFromString(emissive, appearances->Attribute("emissive"), 4);
		extractElementsFromString(ambient, appearances->Attribute("ambient"), 4);
		extractElementsFromString(diffuse, appearances->Attribute("diffuse"), 4);
		extractElementsFromString(specular, appearances->Attribute("specular"), 4);
		appearances->QueryFloatAttribute("shininess", &shininess);

		cout << "\n\t- ID: " << id << endl;
		cout << "\temissive: ";
		for (unsigned int i = 0; i < emissive.size(); i++)
			cout << emissive[i] << " ";
		cout << "\n\tambient: ";
		for (unsigned int i = 0; i < ambient.size(); i++)
			cout << ambient[i] << " ";
		cout << "\n\tdiffuse: ";
		for (unsigned int i = 0; i < diffuse.size(); i++)
			cout << diffuse[i] << " ";
		cout << "\n\tspecular: ";
		for (unsigned int i = 0; i < specular.size(); i++)
			cout << specular[i] << " ";
		cout << "\n\tshininess: " << shininess;

		Appearance *a = new Appearance(id, &ambient[0], &diffuse[0], &specular[0], &emissive[0], shininess);

		//se textureref existir, tem de haver texlength_s e texlength_t
		if (appearances->Attribute("textureref") != NULL) 
		{
			textureref = appearances->Attribute("textureref");		
			cout << "\n\ttextureref: " << textureref;
			a->setTexture(scene.getTextureByID(textureref)); //get previously set texture by nodeID
			appearances->QueryFloatAttribute("texlength_s", &texlength_s);
			cout << "\n\ttexlength_s: " << texlength_s;
			appearances->QueryFloatAttribute("texlength_t", &texlength_t);
			cout << "\n\ttexlength_t: " << texlength_t;
			if(!texlength_s || !texlength_t)
				throw("Texture wrap not set or invalid");
			a->setTextureWrap(texlength_s, texlength_t);
		}

		scene.addAppearance(a);

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
						for (unsigned int i = 0; i < transforms_components.size(); i++)						
							cout << transforms_components[i] << " ";
						cout << endl;

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
						for (unsigned int i = 0; i < transforms_components.size(); i++)
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
							for (unsigned int i = 0; i < xy1.size(); i++)
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
	this->scene = * new Scene();

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