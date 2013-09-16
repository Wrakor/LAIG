#ifndef CG_CGFinterface
#define CG_CGFinterface

/**
 * @file CGFinterface.h
 * CGF class to implement the interface for CGF applications.
 * 
 */

#include <GL/glui.h>

// forward declaration of scene
class CGFscene;

class CGFinterface 
/// This class is used to handle the visual interface as well as the keyboard and mouse handlers for CGF applications. It is mostly based on GLUI.
{
	public:
		CGFinterface();		///< Base constructor
		virtual ~CGFinterface();	///< Base destructor

		// Initialization methods

		void init(int parent);	///< Initializes the interface, in terms of handler setup and other low-level operations. Should not be overriden by subclasses
		virtual void initGUI(); ///< Initializes the graphical interface itself, i.e. creating controls and establishing relations with the scene. Should be overriden by subclasses. Default is empty.

		// Link to scene
		void setScene(CGFscene *scene);		///< Links this interface to the CGFscene _scene_, so that the interface can access it.

		// Active interface selection

		static void setActiveInterface(CGFinterface *gli)		///< registers _gli_ as the active interface 
		{ activeInterface=gli; }; 

		/** @name Static input (keyboard, mouse) handlers to be registered with GLUT
		 * These methods are global handlers that are registered by the application as the keyboard and mouse handlers.
		 * When invoked, they will route execution to the corresponding instance methods of the active CGFinterface instance.
		 */
		//@{
		static void preprocessKeyboard(unsigned char key, int x, int y);	///< Called when a key is pressed. Receives as parameters the key pressed, and the mouse coordinates at the time of the press.
		static void preprocessMouse(int button, int state, int x, int y);	///< Called when a mouse button event occurs. 
		static void preprocessMouseMoved(int x, int y);						///< Called when the mouse moves while *some* mouse button is pressed
		static void preprocessPassiveMouseMoved(int x, int y);				///< Called when the mouse moves while *no* mouse button is pressed
		//@}

		// Handlers to be overridden by subclasses

		/** @name Instance input handlers to be implemented by CGFinterface subclasses
		 * These methods are to be implemented by each interface class, and will be called indirectly 
		 * via the static handlers *preprocess\** when the corresponding events occur.
		 */
		//@{
		virtual void processKeyboard(unsigned char key, int x, int y);		///< Called when a key is pressed. Receives as parameters the key pressed, and the mouse coordinates at the time of the press.
		virtual void processMouse(int button, int state, int x, int y);		///< Called when a mouse button event occurs. 
		virtual void processMouseMoved(int x, int y);						///< Called when the mouse moves while *some* mouse button is pressed
		virtual void processPassiveMouseMoved(int x, int y);				///< Called when the mouse moves while *no* mouse button is pressed
		//@}

		static void preprocessGUI(GLUI_Control *ctrl);	///< Static handler to be registered as callback for every control created. When a control is interacted with, this function will route execution to the active interface's processGUI() method, providing a pointer to the control.
		virtual void processGUI(GLUI_Control *ctrl);	///< Instance method that processes a GUI event originating from _ctrl_.
		void syncVars();								///< Synchronizes the visible controls to reflect changes in associated variables (e.g. change the position of a scroll bar due to a variable increment in the code). Called by CGFapplication::gluiIdleFunction().

		/** @name Control creation methods (GLUI-based)
		 * These methods are wrappers around the corresponding GLUI functions.
		 * Please see the \gluidocs for details.
		 */
		//@{
		GLUI_Checkbox* addCheckbox(char* name, int* value = (int*) 0, int id = -1);
		GLUI_Checkbox* addCheckboxToPanel(GLUI_Panel *p,char* name, int* value= (int*) 0,int id = -1);

		GLUI_Button* addButton(char* name,int id = -1);
		GLUI_Button* addButtonToPanel(GLUI_Panel *p,char* name,int id = -1);

		void addColumn();
		void addColumnToPanel(GLUI_Panel *p);

		GLUI_EditText* addEditText(char* name, char* var = (char*) 0,int id = -1);
		GLUI_EditText* addEditText(char* name, int* var = (int*) 0,int id = -1);
		GLUI_EditText* addEditText(char* name, float* var = (float*) 0,int id = -1);
		GLUI_EditText* addEditTextToPanel(GLUI_Panel *p,char* name, char* var = (char*) 0,int id = -1);
		GLUI_EditText* addEditTextToPanel(GLUI_Panel *p,char* name, int* var= (int*) 0,int id = -1);
		GLUI_EditText* addEditTextToPanel(GLUI_Panel *p,char* name, float* var= (float*) 0,int id = -1);

		GLUI_Listbox* addListbox(char* name, int* var= (int*) 0, int id = -1);
		GLUI_Listbox* addListboxToPanel(GLUI_Panel *p,char* name, int* var= (int*) 0, int id = -1);

		GLUI_Panel* addPanel(char* name, int type = 1);
		GLUI_Panel* addPanelToPanel(GLUI_Panel *p,char* name, int type = 1);

		GLUI_RadioButton* addRadioButtonToGroup(GLUI_RadioGroup * group, char * name);

		GLUI_RadioGroup* addRadioGroup(int *var, int id=-1);
		GLUI_RadioGroup* addRadioGroupToPanel(GLUI_Panel* p,int *var= (int*) 0, int id=-1);

		GLUI_Rollout* addRollout(char *name, int open=1, int type =1);
		GLUI_Rollout* addRolloutToPanel(GLUI_Panel* p,char *name, int open=1, int type =1);

		GLUI_Rotation* addRotation(char* name, float* var= (float*) 0, int id=-1);
		GLUI_Rotation* addRotationToPanel(GLUI_Panel* p,char* name, float* var= (float*) 0, int id=-1);

		void addSeparator();
		void addSeparatorToPanel(GLUI_Panel * p);

		GLUI_Spinner* addSpinner(char* name, int type = 2, int* var= (int*)0, int id=-1);
		GLUI_Spinner* addSpinnerToPanel(GLUI_Panel* p,char* name, int type = 2, int* var= (int*)0, int id=-1);

		GLUI_StaticText* addStaticText(char* name);
		GLUI_StaticText* addStaticTextToPanel(GLUI_Panel *p,char* name);

		GLUI_Translation* addTranslation(char* name, int type = GLUI_TRANSLATION_X, float* var = (float*) 0, int id=-1);
		GLUI_Translation* addTranslationToPanel(GLUI_Panel* p,char* name, int type = GLUI_TRANSLATION_X, float* var= (float*) 0, int id=-1);
		//@}

	protected:

		CGFscene *scene;		///< Pointer to the scene linked to this interface. This is initialized by CGFapplication::setInterface(), which invokes CGFinterface::setScene() with the previously registered scene as parameter.

		static int modifiers;	///< Stores the state of modifier keys (CTRL, ALT, SHIFT) for use in mouse and keyboard event handlers
		static CGFinterface * activeInterface;
		GLUI* glui_window;


		// state and control variables to handle mouse interaction
		float displacementX;
		float displacementY;
		bool pressing_left;
		bool pressing_middle;
		bool pressing_right;
		float prev_X;
		float prev_Y;
};


#endif
