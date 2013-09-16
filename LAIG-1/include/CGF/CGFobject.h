#ifndef CG_CGFobject
#define CG_CGFobject

/**
 * @file CGFobject.h
 * Object class of CGF.
 * 
 */

#include <stdlib.h>
#include <GL/glut.h>

class CGFobject 
/// This class should be used as a base class for all the objects within a CGFscene, by reimplementation of its draw() method.
{
	public:
		CGFobject();		///< Base contructor
		virtual ~CGFobject();		///< Base destructor

		virtual void draw(); ///< Virtual function that every object should have implemented
};

#endif
