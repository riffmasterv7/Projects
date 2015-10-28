
#include "gles.h"

#include "EShaderType.h"


const char * shaderTypeToString(int32 type)
{
	static const char * shaderTypesText[E_SHADER_TYPE_COUNT] =
	{
		"VERTEX",
		"FRAGMENT",
		"COMPUTE",
		"GEOMETRY",
		"TESSCTRL",
		"TESSEVAL"
	};

	return shaderTypesText[type];
}

const int32 shaderTypeToGLESType(int32 type)
{
	static int32 glShaderTypes[E_SHADER_TYPE_COUNT] =
	{
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_COMPUTE_SHADER,
		GL_GEOMETRY_SHADER_OES,
		GL_TESS_CONTROL_SHADER_OES,
		GL_TESS_EVALUATION_SHADER_OES
	};

	return glShaderTypes[type];
}

const int32 shaderTypeToGLESMask(int32 type)
{
	static int32 glShaderBits[E_SHADER_TYPE_COUNT] =
	{
		GL_VERTEX_SHADER_BIT,
		GL_FRAGMENT_SHADER_BIT,
		GL_COMPUTE_SHADER_BIT,
		GL_GEOMETRY_SHADER_BIT_OES,
		GL_TESS_CONTROL_SHADER_BIT_OES,
		GL_TESS_EVALUATION_SHADER_BIT_OES
	};

	return glShaderBits[type];
}

const char * shaderTypeToFileExtension(int32 type)
{
	static const char * shaderFileExtension[E_SHADER_TYPE_COUNT] =
	{
		".vert",
		".frag",
		".comp",
		".geom",
		".ctrl",
		".eval"
	};

	return shaderFileExtension[type];
}


