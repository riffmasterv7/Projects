precision highp float;

attribute vec2 a_attrib0;
attribute vec3 a_attrib1;

varying highp vec3 v_LPR;

void main() { 

	gl_Position = vec4(a_attrib0, 0.0, 1.0);
	v_LPR = a_attrib1;
}
