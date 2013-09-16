#ifndef _CGF_SHADER_H_
#define _CGF_SHADER_H_

/**
 * @file CGFshader.h
 * Shader class of CGF. Based on Swiftless tutorial at http://www.swiftless.com/tutorials/glsl/2_validation.html
 * 
 */

class CGFshader
/// Shader class for loading, storing, compiling, applying and updating GLSL shaders (vertex and fragment).
{
public:
	CGFshader(void);										///< Base constructor
    CGFshader(const char *vsFile, const char *fsFile);		///< Main constructor. Loads and validates the vertex and fragment shaders specified by _vsFile_ and _fsFile_, using init().
	virtual ~CGFshader(void);										///< Base destructor

    virtual void init(const char *vsFile, const char *fsFile);		///< Loads and validates the vertex and fragment shaders specified by _vsFile_ and _fsFile_, using init().
    
	virtual void update();											///< Updates the shader parameters (uniforms). In this base implementation, it updates the _time_ uniform (if it exists) with CGFapplication::getTime().
	virtual void update(float time);								///< Updates the _time_ uniform (if it exists) with the value _time_.
	virtual void bind();											///< Binds (activates) the shader.
	virtual void unbind();											///< Unbinds (deactivates) the shader, and reactivates the fixed-function pipeline
	
	unsigned int id();										///< Returns the shader id
	
private:
	unsigned int shader_id;	///< Shader id
	unsigned int shader_vp;	///< Vertex shader pointer
	unsigned int shader_fp; ///< Fragment shader pointer

	int timeloc;			///< Identifier (Location) for the _time_ uniform.
};

#endif //_CGF_SHADER_H_
