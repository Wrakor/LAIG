#ifndef CG_CGFlight
#define CG_CGFlight

/**
 * @file CGFlight.h
 * Light class of the CGF framework.
 * 
 */

#include "CGFobject.h"
#include "CGFappearance.h"
#include <GL/glut.h>


class CGFlight: public CGFobject 
/// The light class is used to initialise, store information and generally handle an _OpenGL_ _GLlight_. It may also draw a sphere to represent the light on the 3D scene.
{
	public:
		CGFlight(unsigned int lightid, float* pos, float *dir=NULL);	///< Constructor. Receives an OpenGL light id GL_LIGHT_n, a position in 4-float vector and an optional direction, both as defined in OpenGL's *glLightfv()* documentation.
		virtual ~CGFlight();		///< Base destructor

		void draw();		///< Draws a visible representation of the light (a sphere). It forces an update() of the light.
		void update();		///< Updates the position and direction of the corresponding OpenGL's light. Has to be called after any camera transformations, so that the light is transformed in the proper referential.
		void enable();		///< Enables the light.
		void disable();		///< Disables the light.

		void setAmbient(float*);		///< Sets the Ambient component of the light (4-float vector)
		void setDiffuse(float*);		///< Sets the Diffuse component of the light (4-float vector)
		void setSpecular(float*);		///< Sets the Specular component of the light (4-float vector)
		void setKc(float f);			///< Sets the constant attenuation factor
		void setKl(float f);			///< Sets the linear attenuation factor
		void setKq(float f);			///< Sets the quadratic attenuation factor
		void setAngle(float a);			///< Sets the angle of the spotlight's cone
		
		static float background_ambient[4];

	protected:
		static CGFappearance* default_light_material;

		unsigned int id;
		float ambient[4];
		float diffuse[4];
		float specular[4];
		float kc;
		float kl;
		float kq;
		float angle;
		bool enabled;

		GLUquadric* glu_quadric;

		float position[4];
		float direction[3];
		CGFappearance* material;

};

#endif
