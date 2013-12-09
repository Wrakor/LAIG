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
#include "Node.h"
#include "Animation.h"

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
			throw "Error parsing, parameter with invalid number/type of attribute(s)!";

		text_ss >> j;
		elements.push_back(j);
	}

	if(text_ss.rdbuf()->in_avail()!=0)
		throw "Error parsing, parameter with invalid number/type of attribute(s)!";
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

		string id = camera->Attribute("id"),type = camera->Value(), target;		
		vector<float> pos_vector;// = {0,0,0};
		vector<float> target_vector;

		if (id.empty() || type.empty())
			throw "Error parsing Cameras: empty string";
		if (camera->QueryFloatAttribute("near", &near) != 0)
			throw "Error parsing Cameras: no 'near' attribute";
		if (camera->QueryFloatAttribute("far", &far) != 0)
			throw "Error parsing Cameras: no 'far' attribute";

		cout << endl << "\tID: " << id << endl;
		cout << "\tType: " << type << endl;
		cout << "\tNear: " << near << endl;
		cout << "\tFar: " << far << endl;

		if (type == "perspective")
		{
			if (camera->QueryFloatAttribute("angle", &angle) != 0)
				throw "Error parsing Cameras: no 'angle' attribute";

			extractElementsFromString(pos_vector, camera->Attribute("pos"), 3);
			extractElementsFromString(target_vector, camera->Attribute("target"), 3);

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
			if (camera->QueryFloatAttribute("left", &left) != 0)
				throw "Error parsing Cameras: no 'left' attribute";
			if (camera->QueryFloatAttribute("right", &right) != 0)
				"Error parsing Cameras: no 'right' attribute";
			if (camera->QueryFloatAttribute("top", &top) != 0)
				throw "Error parsing Cameras: no 'top' attribute";
			if (camera->QueryFloatAttribute("bottom", &bottom) != 0)
				"Error parsing Cameras: no 'bottom' attribute";

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

			if (type.empty() || id.empty())
				throw "Error parsing Lighting: empty string";

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

			Light *l = new Light(id, GL_LIGHT0+j, &location[0]);

			if (type == "spot")
			{
				if (lighting->QueryFloatAttribute("angle", &angle) != 0)
					throw "Error parsing Lighting: no 'angle' attribute!";
				if (lighting->QueryFloatAttribute("exponent", &exponent) != 0)
					throw "Error parsing Lighting: no 'exponent' attribute!";
				extractElementsFromString(direction, lighting->Attribute("direction"), 3);

				cout << "\tangle:" << angle << endl;
				cout << "\texponent:" << exponent << endl;
				for (unsigned int i = 0; i < direction.size(); i++)
					cout << "\tdirection:" << direction[i] << endl;

				//got to create again to set direction and exponent
				l = new Light(id, GL_LIGHT0+j, &location[0], &direction[0], exponent);
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
	string id, file_name;
	texturesElement = yafElement->FirstChildElement( "textures" );

	if(!texturesElement)
		throw "Error parsing textures";

	cout << "\nTextures" << endl;

	TiXmlElement *textures = texturesElement->FirstChildElement("texture");

	while (textures)
	{
		id = textures->Attribute("id");
		file_name = textures->Attribute("file");

		if (id.empty() || file_name.empty())
			throw "Error parsing textures: empty string!";
		cout << "\tID: " << id << endl;
		cout << "\tFile: " << file_name << endl << endl;

		this->scene.addTexture(new Texture(id, this->path+file_name)); //usa o caminho do yaf para as texturas

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
		if (id.empty())
			throw "Error parsing appearances: empty string";

		extractElementsFromString(emissive, appearances->Attribute("emissive"), 4);
		extractElementsFromString(ambient, appearances->Attribute("ambient"), 4);
		extractElementsFromString(diffuse, appearances->Attribute("diffuse"), 4);
		extractElementsFromString(specular, appearances->Attribute("specular"), 4);

		if (appearances->QueryFloatAttribute("shininess", &shininess) != 0)
			throw "Error parsing appearances: no 'shininess' attribute";

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
			if (textureref.empty())
				throw "Error parsing appearanes: no 'textureref' attribute";
			if (appearances->QueryFloatAttribute("texlength_s", &texlength_s) != 0)
				throw "Error parsing appearances: no 'textlength_s' attribute";
			if (appearances->QueryFloatAttribute("texlength_t", &texlength_t) != 0)
				throw "Error parsing apearances: no 'length_t' attribute";
			cout << "\n\ttextureref: " << textureref;
			cout << "\n\ttexlength_s: " << texlength_s;
			cout << "\n\ttexlength_t: " << texlength_t;

			a->setTexture(scene.getTextureByID(textureref)); //get previously set texture by nodeID
			if(!texlength_s || !texlength_t)
				throw("Texture wrap not set or invalid");
			a->setTextureWrap(texlength_s, texlength_t);
		}

		scene.addAppearance(a);

		appearances = appearances->NextSiblingElement();
	}
}

void Parser::parseAnimations()
{
	animationsElement = yafElement->FirstChildElement("animations");

	if (!animationsElement)
		throw "Animations element doesn't exist!";
	else
	{
		TiXmlElement *animation = animationsElement->FirstChildElement();

		while (animation)
		{		
			string id, type;
			float span, xx, yy, zz;

			id = animation->Attribute("id");
			if (id.empty())
				throw "Error parsing animation: no id attribute";

			if (animation->QueryFloatAttribute("span", &span) != 0)
				throw "Error parsing animation: no span attribute";

			type = animation->Attribute("type");
			if (type.empty())
				throw "Error parsing animation: no type attribute";

			if (type == "linear")
			{
				LinearAnimation *readAnimation = new LinearAnimation(id, span);

				TiXmlElement *controlpoint = animation->FirstChildElement();

				if (!controlpoint)
					throw "Error parsing animation: at least one control point is needed!"; //two i guess

				while (controlpoint)
				{
					if (controlpoint->QueryFloatAttribute("xx", &xx) != 0 || controlpoint->QueryFloatAttribute("yy", &yy) || controlpoint->QueryFloatAttribute("zz", &zz))
						throw "Error parsing animation: no controlpoint attributes";

					std::array<float, 3> readControlPoint;
					readControlPoint[0] = xx;
					readControlPoint[1] = yy;
					readControlPoint[2] = zz;

					readAnimation->addControlPoint(readControlPoint);

					controlpoint = controlpoint->NextSiblingElement();
				}

				if (readAnimation->getNumControlPoints() < 2)
					throw "Error parsing animation: you must define at least 2 control points";

				this->scene.addAnimation(readAnimation);

				animation = animation->NextSiblingElement();
			}
			else
				throw "Only linear animations supported";
		}
	}
}

void Parser::parseGraph()
{
	graphElement = yafElement->FirstChildElement( "graph" );

	if(!graphElement)
		throw "Error parsing graph";

	string rootid, id;
	bool displayList;

	rootid = graphElement->Attribute("rootid");

	if(rootid.empty())
		throw "Error parsing graph attributes";



	cout << "\nGraph" << endl;
	cout << "\tRoot ID: " << rootid << endl;

	this->scene.rootNode = rootid;

	TiXmlElement *node = graphElement->FirstChildElement("node");

	if (!node)
		throw "Error parsing nodes!";
	else
		while (node)
		{
			id = node->Attribute("id");
			if (id.empty())
				throw "Error parsing node: no 'id' attribute";
			cout << "\n\tID: " << id << endl;

			TiXmlElement *transforms = node->FirstChildElement("transforms");
			TiXmlElement *transformsElement = transforms->FirstChildElement();

			Node *readNode = new Node(id);

			const char * readDisplayList = node->Attribute("displaylist");
			if (readDisplayList != NULL)
			{
				displayList = to_bool(readDisplayList);
				readNode->displayList = displayList;
				cout << "\tDisplaylist: " << boolalpha << displayList << endl;
			}

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			cout << "\t-Transforms:" << endl;

			if (!transforms)
				throw "Error parsing transforms";
			else
				while (transformsElement)
				{
					vector<float> transforms_components, scale;
					float angle;
					string axis, value = transformsElement->Value();

					if (value.empty())
						throw "error parsing node: no tag value";
					else if (value  == "translate")
					{
						extractElementsFromString(transforms_components, transformsElement->Attribute("to"), 3);

						cout << "\tTranslate: ";
						for (unsigned int i = 0; i < transforms_components.size(); i++)						
							cout << transforms_components[i] << " ";
						cout << endl;

						glTranslatef(transforms_components[0],transforms_components[1],transforms_components[2]);
					}
					else if (value == "rotate")
					{
						axis = transformsElement->Attribute("axis");
						if (axis.empty())
							throw "Error parsing node: no 'axis' attribute";

						if (transformsElement->QueryFloatAttribute("angle", &angle) != 0)
							throw "Error parsing node: no 'angle' attribute";

						cout << "\tRotate axis " << axis << " by " << angle <<" degrees" << endl;

						if(axis=="x")
							glRotatef(angle, 1, 0, 0);
						else if(axis=="y")
							glRotatef(angle, 0, 1, 0);
						else if(axis=="z")
							glRotatef(angle, 0, 0, 1);
						else
							throw "Invalid axis";
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
						glScalef(transforms_components[0], transforms_components[1], transforms_components[2]);
					}

					transformsElement = transformsElement->NextSiblingElement();
				}

				glGetFloatv(GL_MODELVIEW_MATRIX, readNode->T);

				string appearancerefid, value;
				TiXmlElement *children = transforms->NextSiblingElement("children"), *appearanceref = transforms->NextSiblingElement("appearanceref"), *animationref = transforms->NextSiblingElement("animationref"); //pode ser children ou appearanceref ou animationref	
				if (!children)
					throw "Error parsing node: no children block";

				if (appearanceref)
				{
					appearancerefid = appearanceref->Attribute("id");
					if (appearancerefid.empty())
						throw "Error parsing appearanceref id";
					cout << "\t-Appearanceref ID: " << appearancerefid << endl;

					readNode->appearance=this->scene.getAppearanceByID(appearancerefid);
				}

				if (animationref)
				{
					string animationrefid = animationref->Attribute("id");
					if (animationrefid.empty())
						throw "Error parsing animationref id";
					cout << "\t-Animationref ID: " << animationrefid << endl;

					readNode->animation = this->scene.getAnimationByID(animationrefid);
				}

				TiXmlElement *childrenElement = children->FirstChildElement();				

				cout << "\t-Children:" << endl;
				while (childrenElement)
				{
					value = childrenElement->Value();
					//cout << "\tVALUE: " << value << endl;

					if (value.empty())
						throw "Error parsing node: no children 'value' set"; 
					if (value == "rectangle")
					{
						vector<float> xy1, xy2;

						extractElementsFromString(xy1, childrenElement->Attribute("xy1"), 2);
						extractElementsFromString(xy2, childrenElement->Attribute("xy2"), 2);

						cout << "\tRectangle: ";
						for (unsigned int i = 0; i < xy1.size(); i++)
							cout << xy1[i] << " ";
						for (unsigned int i = 0; i < xy2.size(); i++)
							cout << xy2[i] << " ";
						cout << endl;

						readNode->primitivas.push_back(new Rectangle(xy1[0],xy2[0],xy1[1],xy2[1]));

					}
					else if (value == "triangle")
					{
						vector<float> xyz1, xyz2, xyz3;

						extractElementsFromString(xyz1, childrenElement->Attribute("xyz1"),	3);
						extractElementsFromString(xyz2, childrenElement->Attribute("xyz2"),	3);
						extractElementsFromString(xyz3, childrenElement->Attribute("xyz3"),	3);

						readNode->primitivas.push_back(new Triangle(xyz1[0],xyz2[0],xyz3[0],xyz1[1],xyz2[1],xyz3[1],xyz1[2],xyz2[2],xyz3[2]));
					}
					else if (value == "cylinder")
					{
						float base, top, height;
						int slices, stacks;

						if (childrenElement->QueryFloatAttribute("base", &base) != 0)
							throw "Error parsing cylinder: no 'base' attribute";
						if (childrenElement->QueryFloatAttribute("top", &top) != 0)
							throw "Error parsing cylinder: no 'top' attribute";
						if (childrenElement->QueryFloatAttribute("height", &height) != 0)
							throw "Error parsing cylinder: no 'height' attribute";
						if (childrenElement->QueryIntAttribute("slices", &slices) != 0)
							throw "Error parsing cylinder: no 'slices' attribute";
						if (childrenElement->QueryIntAttribute("stacks", &stacks) != 0)
							throw "Error parsing cylinder: no 'stacks' attribute";

						readNode->primitivas.push_back(new Cylinder(base, top, height, slices, stacks));
					}
					else if (value == "sphere")
					{
						float radius;
						int slices, stacks;

						if (childrenElement->QueryFloatAttribute("radius", &radius) != 0)
							throw "Error parsing sphere: no 'radius' attribute";
						if (childrenElement->QueryIntAttribute("slices", &slices) != 0)
							throw "Error parsing sphere: no 'slices' attribute";
						if (childrenElement->QueryIntAttribute("stacks", &stacks) != 0)
							throw "Error parsing sphere: no 'stacks' attribute";

						readNode->primitivas.push_back(new Sphere(radius, slices, stacks));
					}
					else if (value == "torus")
					{
						float inner, outer;
						int slices, loops;

						if (childrenElement->QueryFloatAttribute("inner", &inner) != 0)
							throw "Error parsing torus: no 'inner' attribute";
						if (childrenElement->QueryFloatAttribute("outer", &outer) != 0)
							throw "Error parsing torus: no 'outer' attribute";
						if (childrenElement->QueryIntAttribute("slices", &slices) != 0)
							throw "Error parsing torus: no 'slices' attribute";
						if (childrenElement->QueryIntAttribute("loops", &loops) != 0)
							throw "Error parsing torus: no 'loops' attribute";

						readNode->primitivas.push_back(new Torus(inner, outer, slices, loops));
					}
					else if (value == "noderef")
					{
						string childid = childrenElement->Attribute("id");
						if (childid.empty())
							throw "Error parsing node: no noderef 'id' attribute";

						cout << "\tnoderef id:" << childid << endl;

						readNode->children.push_back(childid);
					}
					else if (value == "plane")
					{
						int parts;

						if (childrenElement->QueryIntAttribute("parts", &parts) != 0)
							throw "Error parsing node: no plane 'parts' attribute";			

						readNode->primitivas.push_back(new Plane(parts));
					}
					else if (value == "patch")
					{
						int order, partsU, partsV;	
						string compute = childrenElement->Attribute("compute");

						if (childrenElement->QueryIntAttribute("order", &order) != 0 ||	childrenElement->QueryIntAttribute("partsU", &partsU) != 0 || childrenElement->QueryIntAttribute("partsV", &partsV) || compute.empty())
							throw "Error parsing patch attributes";

						TiXmlElement *controlpoint = childrenElement->FirstChildElement();
						float xx, yy, zz;

						Patch *patch = new Patch(order, partsU, partsV, compute);

						while (controlpoint)
						{
							if (controlpoint->QueryFloatAttribute("xx", &xx) != 0 || controlpoint->QueryFloatAttribute("yy", &yy) || controlpoint->QueryFloatAttribute("zz", &zz))
								throw "Error parsing animation: no controlpoint attributes";

							patch->addControlPoint(xx, yy, zz);

							controlpoint = controlpoint->NextSiblingElement();
						}

						readNode->primitivas.push_back(patch);
					}
					else if (value == "vehicle")
					{
						readNode->primitivas.push_back(new Vehicle);
					}
					else if (value == "waterline")
					{
						string heightmap = childrenElement->Attribute("heightmap"), texturemap = childrenElement->Attribute("texturemap"), fragmentshader = childrenElement->Attribute("fragmentshader"), vertexshader = childrenElement->Attribute("vertexshader");

						if (heightmap.empty() || texturemap.empty() || fragmentshader.empty() || vertexshader.empty())
							throw "Error parsing waterline attributes";

						readNode->primitivas.push_back(new Waterline(heightmap, texturemap, fragmentshader, vertexshader));
					}
					else if (value == "mountain")
					{
						string heightmap = childrenElement->Attribute("heightmap"), texturemap = childrenElement->Attribute("texturemap"), fragmentshader = childrenElement->Attribute("fragmentshader"), vertexshader = childrenElement->Attribute("vertexshader");

						if (heightmap.empty() || texturemap.empty() || fragmentshader.empty() || vertexshader.empty())
							throw "Error parsing waterline attributes";

						readNode->primitivas.push_back(new Mountain(heightmap, texturemap, fragmentshader, vertexshader));
					}
					else if (value == "board")
					{
						int size;

						if (childrenElement->QueryIntAttribute("size", &size) != 0)
							throw "Error parsing Board: no 'size' attribute";

						Tabuleiro *board = new Tabuleiro(size);
						readNode->primitivas.push_back(board);
						scene.board = board;
					}

					childrenElement = childrenElement->NextSiblingElement();
				}			
				this->scene.addNode(id, readNode);
				node = node->NextSiblingElement();
		}
}

Parser::Parser(char *filename)
{
	string path = filename;
	unsigned found = path.find_last_of("/\\");
	if(found!=string::npos)
		this->path = path.substr(0,found+1); //guarda o caminho para o ficheiro yaf (para usar no caminho das texturas)
	//INIT Scene
	this->scene = * new Scene();

	// Read XML from file

	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	if ( !loadOkay )
	{
		string message = "Could not load file '";
		message+=filename;
		message+="'!. Error='";
		message+=doc->ErrorDesc();
		message+="'. Exiting.\n";
		throw message;
	}

	yafElement = doc->FirstChildElement( "yaf" );

	if (yafElement == NULL)
		throw "Main yaf block element not found! Exiting!\n";

	parseGlobals();
	parseCameras();
	parseLighting();
	parseTextures();	
	parseAppearances();
	parseAnimations();
	parseGraph();
	interface.setScene(&this->scene);
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
	if (str.empty())
		throw "Error parsing: empty string";

	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}