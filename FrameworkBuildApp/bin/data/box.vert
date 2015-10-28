#version 300 es

#extension GL_EXT_gpu_shader5 : enable

precision highp float;

layout (location = 0) in vec4 VS_ATTR_0;

out vec4 GS_PositionHeight;

void main() 
{ 
	GS_PositionHeight = VS_ATTR_0;
}
