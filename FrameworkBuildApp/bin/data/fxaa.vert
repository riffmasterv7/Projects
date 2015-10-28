
precision mediump float;

attribute vec2 a_attrib0;
attribute vec2 a_attrib1;

varying vec2 v_texcoordNW;
varying vec2 v_texcoordNE;
varying vec2 v_texcoordSW;
varying vec2 v_texcoordSE;

varying vec2 v_texcoord00;

float FXAA_SUB_PIXEL_SHIFT = 0.43;

uniform vec2 u_rtRcpSize;

void main(void) {
	gl_Position = vec4(a_attrib0, 0.0, 1.0);
  
	v_texcoord00 = a_attrib1;
	
	vec2 shift = u_rtRcpSize * FXAA_SUB_PIXEL_SHIFT;
	v_texcoordNW = a_attrib1 - shift;
	v_texcoordSE = a_attrib1 + shift;
	v_texcoordNE = vec2(v_texcoordSE.x, v_texcoordNW.y);
	v_texcoordSW = vec2(v_texcoordNW.x, v_texcoordSE.y);
}