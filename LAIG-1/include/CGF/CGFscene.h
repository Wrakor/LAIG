#ifndef CG_CGFscene
#define CG_CGFscene

/**
 * @file CGFscene.h
 * Scene class of CGF.
 * 
 */

#include <list>
#include <vector>

#include "CGFobject.h"
#include "CGFaxis.h"
#include "CGFlight.h"
#include "CGFcamera.h"
#include "CGFinterface.h"

class CGFscene
/// This class stores information on one 3D scene, including cameras, lights and objects.
{
	friend class CGFapplication;
	friend class CGFinterface;
	public:
		CGFscene();
		virtual ~CGFscene();


		virtual void init();					///< Initializes the scene. It is called by CGFapplication when the scene is associated through CGFapplication::setScene()
		virtual void display();					///< Renders the scene. When this scene is active in the current application, it is called by CGFapplication::display(), which in turn is registered as GLUT display handler, thus being called at every frame refresh.
		virtual void update(unsigned long millis);		///< Periodic update function. This is a generic update handler for use in animations. It is called periodically at the time interval set with setUpdatePeriod(). When called, receives as parameter the actual timestamp in millisseconds, to be used in animations and other computations.

		CGFaxis axis;
		
		virtual void initCameras();				///< Initializes default cameras. Usually called at the end of the constructor.
		void activateCamera(int i);				///< If a default set of cameras is available in scene_cameras, activates the camera at position _i_

		void setInterface(CGFinterface *i);		///< Registers a CGFinterface instance with this scene. This is typically called by CGFapplication when a CGFinterface is registered through CGFapplication::setInterface()

	protected:
		void setUpdatePeriod(unsigned long millis);		///< Sets the update period between each call to update(), in milliseconds. if millis=0, updates are disabled.

		void checkUpdate();						///< Called by the main application class, that handles timing, to check if an update is needed due to timeout. If so, it calls update(), and updates the timer.
		unsigned long updatePeriod;
		unsigned long lastUpdate;

		CGFinterface *iface;

		std::list<CGFlight*> scene_lights;
		std::list<CGFobject*> scene_objects;
		std::vector<CGFcamera*> scene_cameras;

		CGFcamera* activeCamera;

};

#endif
