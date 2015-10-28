
#ifndef _ESHADERTYPE_
#define _ESHADERTYPE_

#include <types_v7.h>

enum EShaderType
{
	E_VERTEX_SHADER,
	E_FRAGMENT_SHADER,
	E_COMPUTE_SHADER,
	E_GEOMETRY_SHADER,
	E_TESSCTRL_SHADER,
	E_TESSEVAL_SHADER,

	E_SHADER_TYPE_COUNT
};

const char* shaderTypeToString(int32 type);
const int32 shaderTypeToGLESType(int32 type);
const int32 shaderTypeToGLESMask(int32 type);
const char* shaderTypeToFileExtension(int32 type);


#endif