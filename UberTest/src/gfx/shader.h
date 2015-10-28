
#ifndef _SHADER_
#define _SHADER_

#include <types_v7.h>

typedef struct _Shader {
	uint32 id;

	struct {
		uint32 attribs : 3;
		uint32 pad : 29;
	};
} Shader;

int32 shaderCreate(Shader * pShader, const char * pName);

void shaderBind(Shader * pShader);

void shaderDestroy(Shader * pShader);

#endif