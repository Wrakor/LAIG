#include "Animation.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Animation::Animation(string nodeID)
{
	this->nodeID = nodeID;
	this->startTime = 0;
}

//inicializa o startTime com o valor de tempo actual
void Animation::init(float timestamp)
{
	this->startTime = timestamp;
}

LinearAnimation::LinearAnimation(string nodeID, float span) : Animation(nodeID)
{
	this->span = span;
}

void LinearAnimation::init(float timestamp)
{
	Animation::init(timestamp); //contrutor classe pai
	this->lastTimestamp = timestamp; //�ltimo timestamp � inicializado com o tempo actual
	this->timePerControlPoint = span / (controlPoints.size() - 1); //tempo por ponto de controlo � o tempo total a divid pelo n�mero de pontos de controlo -1 (ex: 5 control points d�o 4 sequ�ncias de anima��o)
	changeControlPoint(0); //primeiro control point
}

//adiciona um ponto de controlo ao vector
void LinearAnimation::addControlPoint(array<float, 3> controlPoint) 
{
	this->controlPoints.push_back(controlPoint);
}

//actualiza a anima��o
void LinearAnimation::update(float timestamp)
{
	float animationTime = (timestamp - startTime)/CLOCKS_PER_SEC; //tempo na anima��o em segundos = tempo actual menos tempo quando iniciou
	float timeSinceLastUpdate = (timestamp - lastTimestamp) / CLOCKS_PER_SEC; //tempo desde o ultimo update = tempo actual menos tempo da ultima chamada � fun��o update
	this->timeInThisControlPoint += timeSinceLastUpdate; //actualiza acumulador de tempo neste control point
	if (animationTime<span) //se anima��o ainda n�o acabou
	{
		if (timeInThisControlPoint>=timePerControlPoint) //se esgotamos o tempo para este ponto de controlo
		{
			changeControlPoint(currentControlPoint + 1); //passamos ao pr�ximo
		}
		else //se n�o, fazemos o movimento da anima��o
		{
			//incrementa posi��o actual: percentagem do total da dist�ncia a percorrer pelo tempo percorrido neste ponto de controlo
			currentPos[0] += deltaX * (timeSinceLastUpdate / timePerControlPoint); //x
			currentPos[1] += deltaY * (timeSinceLastUpdate / timePerControlPoint); //y
			currentPos[2] += deltaZ * (timeSinceLastUpdate / timePerControlPoint); //z
		}
		lastTimestamp = timestamp; //actualiza �ltimo timestamp
	}
	/*else //reset
	{
		init(); //reinicia tudo
	}*/
}

void LinearAnimation::draw()
{
	update(); //actualiza
	glTranslatef(currentPos[0], currentPos[1], currentPos[2]); //efectua a transla��o
	glRotatef(angle, 0, 1, 0); //roda conforme o �ngulo calculado
}

//calcula �ngulo de rota��o para cada movimento
void LinearAnimation::calculateAngle()
{
	//usa deltaX e deltaZ para calcular �ngulo entre os dois pontos
	angle = atan2(deltaX, deltaZ) * 180 / M_PI;
}

unsigned int LinearAnimation::getNumControlPoints()
{
	return this->controlPoints.size();
}

void LinearAnimation::changeControlPoint(unsigned int cp)
{
	currentControlPoint = cp; //passamos ao pr�ximo
	this->currentPos = controlPoints[cp]; //posi��o actual � a posi��o no primeiro control point
	//calcula dist�ncia a percorrer para este movimento
	deltaX = controlPoints[currentControlPoint + 1][0] - controlPoints[currentControlPoint][0];
	deltaY = controlPoints[currentControlPoint + 1][1] - controlPoints[currentControlPoint][1];
	deltaZ = controlPoints[currentControlPoint + 1][2] - controlPoints[currentControlPoint][2];
	calculateAngle(); //recalcula angulo para pr�ximo movimento
	this->timeInThisControlPoint = 0; //reinicia acumulador
}