#version 310 es

precision highp float;

#extension GL_EXT_gpu_shader5 : enable

in vec3 FS_Color;
in vec3 FS_Normal;

out vec4 outColor;

void main() 
{ 
	outColor = vec4(FS_Color * dot(FS_Normal, vec3(0.0, 0.0, 1.0)), 1.0);
}