#ifndef CG_CGFtexture
#define CG_CGFtexture

/**
 * @file CGFtexture.h
 * Texture class of CGF.
 * 
 */

#include <vector>
#include <string>

using namespace std;

class CGFtexture
/// This class is used to load, store and refer textures in general. It is meant to be used in CGFappearance instances.
{
	public:
		CGFtexture();					///< Basic contructor
		CGFtexture(string);				///< Constructor for initializing the texture from a file (supports jpeg, png, bmp, among others, using stbi-1.33 - http://nothings.org/stb_image.c )

		virtual ~CGFtexture();

		void apply();					///< Applies the texture (makes it active for whatever is rendered next)

	protected:

		void loadTexture(string);
		string texName;

		int texWidth, texHeight;

		unsigned int texID;
};

#endif
