#ifndef CG_CGFapplication
#define CG_CGFapplication

/**
 * @file CGFapplication.h
 * Main application class of CGF.
 * 
 */
#include "CGFscene.h"
#include "CGFinterface.h"

#include <stdexcept>
#include <string>

class CGFapplication 
/// Main class for an application where you can add scenes, interfaces and manage other aspects of the application.
{
	public:
		CGFapplication();
		~CGFapplication();
		void init(int*, char**); ///< Initial method, should be called from main and have (argc, argv) passed through
		void run();				 ///< Main application loop; should be called after initializing and setting the active scene and interface using _setScene()_ and _setInterface()_
		
		void setScene(CGFscene* gls);			///< Associates the provided scene with the application, and initializes it
		void setInterface(CGFinterface* gli);	///< Associates the provided interface with the application and the previously associated scene, and initializes it

		int getMainWindow();			///< Returns the OpenGL handle to the main application window

		static void snapshot();			///< Takes a snapshot of the application window and stores it in PNG format in the working directory, with a timestamp.

		static float xy_aspect;			///< Aspect ratio of the application window
		static int width;				///< Width of the application window
		static int height;				///< Height of the application window
		static int vpx, vpy, vpw, vph;	///< Viewport coordinates and dimensions within the application window, excluding the interface area.

		static unsigned long getTime();			///< Returns the current time in milliseconds to be used in time-based updates and other actions. The time base may vary from platform to platform, can be from a specific date, or from the start of the application.

		void forceRefresh();			///< Forces the application to refresh, including updating the viewports and corresponding projection matrices

		static CGFapplication *activeApp;

	protected:
		void activate();

		static int app_window;

		CGFscene* app_scene;

		CGFinterface* app_interface;

		static bool refreshRequested;

		void static gluiIdleFunction(void);		///< Static idle handler to be registered in GLUT/GLUI. It is responsible to route execution to any handlers specific of the active application, scene and/or interface.
		void static reshape(int, int);			///< Static reshape handler to be registered in GLUT/GLUI. It is responsible to handle reshape in terms of application, and to update the active camera of the active scene.
		void static display();					///< Static display handler to be registered in GLUT/GLUI. It is responsible to route execution to the active scene's CGFscene::display() handler.
		void environmentInit();					///< Called at the end of init(), it sets the default OpenGL parameters, such as culling, depth testing, etc. (which can later be overriden by CGFscene::init() ).
		void updateScene();						///< 
};

class GLexception: public std::runtime_error 
/// Utility class to handle exceptions specific to the library
{
	public:
		GLexception(const std::string& what_str): runtime_error(what_str) {}

};

#endif
