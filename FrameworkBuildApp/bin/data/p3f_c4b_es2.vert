precision highp float;

uniform mat4 u_mvp;

attribute vec3 a_attrib0;
attribute vec4 a_attrib1;

varying vec4 v_color;

void main() { 

	gl_Position = u_mvp * vec4(a_attrib0, 1.0);

	v_color = a_attrib1;
}
