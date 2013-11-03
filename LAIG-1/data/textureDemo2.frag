uniform sampler2D baseImage;
//uniform sampler2D secondImage;

void main()
{
	vec4 color=texture2D(baseImage, gl_TexCoord[0].st);

	// notice the coordinate conversion to flip the image horizontally and vertically
	//vec4 filter=texture2D(secondImage, vec2(1.0,1.0)-gl_TexCoord[0].st);

	//if (filter.b > 0.5)
	//	color=vec4(0.52,0.18,0.11,1.0);
	
	gl_FragColor = color; 
}
