uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectionMatrix;
	
attribute vec4 gl_Vertex;
	
void main() {
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}