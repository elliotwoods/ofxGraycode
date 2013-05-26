#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect graycode;
uniform sampler2DRect graycodeToBinary;

float viewScale = 1024.0;

int toBinary(int graycode) {
	return (int) (texture2DRect(graycodeToBinary, vec2(graycode, 0.5)).r * 255.0f * 255.0f);
}

void main() {
	vec2 st = gl_TexCoord[0].st;
	vec4 sample = texture2DRect(graycode, st);
	int x = toBinary( (int) sample.r);
	int y = toBinary( (int) sample.g);

	gl_FragColor = vec4(x, y, 0, 1.0 ) / vec4(viewScale, viewScale, 1.0, 1.0);
}