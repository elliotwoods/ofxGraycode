#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect image;
uniform sampler2DRect mean;

uniform int axis;
uniform int bit;

void main()
{
	vec2 st = gl_TexCoord[0].st;
	int high = texture2DRect(image, st).r > texture2DRect(mean, st).r;
	int value = high << bit;

	if (axis == 0)
		gl_FragColor = vec4(value, 0, 0, 1.0 );
	else 
		gl_FragColor = vec4(0, value, 0, 1.0 );
}