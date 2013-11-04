uniform float yScale, zScale, time;
uniform sampler2D hImage;

void main() {				
	gl_TexCoord[0] = gl_MultiTexCoord0;	

	//gl_TexCoord[0].x -= time/8;

	vec4 offset = vec4(0.0,0.0,0.0,0.0);
	vec4 hColor = texture2D(hImage, gl_TexCoord[0].st);
	
		offset.y = yScale*hColor.r;
		offset.z = zScale*hColor.r;

		
	
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + offset);
}     
