#ifndef CG_CGFaxis
#define CG_CGFaxis

/**
 * @file CGFaxis.h
 * CGFobject representing the three axis.
 * 
 * 
 */

#include "CGFobject.h"
#include <GL/glut.h>

#define CG_CGFaxis_DEFAULT_RADBEG 0.1
#define CG_CGFaxis_DEFAULT_RADEND 0.0
#define CG_CGFaxis_DEFAULT_LENGTH 16.0
#define CG_CGFaxis_DEFAULT_SLICES 8
#define CG_CGFaxis_DEFAULT_STACKS 1
#define CG_CGFaxis_DEFAULT_SPHRAD 0.2
#define CG_CGFaxis_DEFAULT_SPHSLI 8
#define CG_CGFaxis_DEFAULT_SPHSTK 16

class CGFaxis: public CGFobject 
/// This class implements three axis to show the use of the CGFobject class.
{
	public:
		CGFaxis();		///< Base constructor. Sets up parameters and creates GLUquadric to use in rendering.
		~CGFaxis();		///< Base destructor
		void draw();	///< Drawing function. Renders three cones of different colors, using GLUquadrics, where the colors are matched to the cone direction: Red cone = X, Green = Y, Blue = Z

	protected:
		double radius_begin;
		double radius_end;
		double lenght;
		int nslices;
		int nstacks;

		double ball_radius;
		int ball_slices;
		int ball_stacks;

		GLUquadric* glu_quadric;
};

#endif
