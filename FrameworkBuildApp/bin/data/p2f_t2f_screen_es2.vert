precision highp float;

attribute vec2 a_attrib0;
attribute vec2 a_attrib1;

varying vec2 v_texcoord;

void main() { 

	gl_Position = vec4(a_attrib0, 0.0, 1.0);
	v_texcoord = a_attrib1;
}
