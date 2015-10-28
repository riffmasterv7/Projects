precision highp float;

uniform mat4 u_mvp;

attribute vec3 a_attrib0;
attribute vec3 a_attrib1;

void main() { 

	gl_Position = u_mvp * vec4(a_attrib0, 1.0);
}
