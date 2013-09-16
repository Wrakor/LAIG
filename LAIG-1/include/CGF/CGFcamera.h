#ifndef CG_CGFcamera
#define CG_CGFcamera

/**
 * @file CGFcamera.h
 * Camera class for the CGF framework.
 *
 */

#define CG_CGFcamera_AXIS_X 0
#define CG_CGFcamera_AXIS_Y 1
#define CG_CGFcamera_AXIS_Z 2

class CGFcamera 
/// This class can be used to implement a point of view for the application.
{
	public:
		CGFcamera();		///< Base constructor
		virtual ~CGFcamera();		///< Base destructor

		virtual void applyView();			///< Applies View transformation (typically used in the beginning of a scene render, in CGFscene::display() )
		virtual void updateProjectionMatrix(int width, int height);		///< Updates the projection matrix. Typically invoked after viewport change, e.g. as a result of a window reshape that triggers CGFapplication::reshape()

		virtual void setExamineMode();		///< Sets the camera to move in Examine mode i.e., to rotate around a fixed point in front of it
		virtual void setWalkMode();			///< Sets the camera to move in Walk mode i.e., to rotate around itself

		virtual void setX(float value);		///< Sets camera X coordinate
		virtual void setY(float value);		///< Sets camera Y coordinate
		virtual void setZ(float value);		///< Sets camera Z coordinate

		/** @name Utility functions for relative/incremental camera movement */
		//@{
		virtual bool rotateTo(int axis, float angle, float increment = 0.5f);	///< Rotates the camera around _axis_ by _increment_ degrees, unless it has reached _angle_ degrees. Useful for stepping a rotation in an animation.
		virtual bool rotate(int axis, float angle);								///< Rotates the camera around _axis_ by _angle_ degrees.
		virtual bool setRotation(int axis, float angle);						///< Sets the rotation around _axis_ to be _angle_ degrees
		virtual bool moveTo(int axis, float value, float increment=0.5f);		///< Moves the camera along _axis_ by _increment_ units, unless _value_ has been reached.
		virtual bool translate(int axis, float value);							///< Moves the camera along _axis_ by _value_ units.
		//@}

	protected:
		float position[3]; //= {0.0,0.0,-25.0};
		float target[3]; //= {0.0,0.0,0.0};
		float rotation[3];// = {0.0,0.0,0.0};

		enum CAMERA_MODE { EXAMINE_MODE, WALK_MODE, TARGET_MODE };

		CAMERA_MODE mode;

};

#endif
