
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "shader.h"
#include "gfx.h"

typedef struct _ShaderSource {
	uint8 * dataSource;
	uint32 dataLength;
} ShaderSource;

static const char * SHADERS_DIR = "../data/";

#define D_MAX_ATTRIBS 8
static uint32 MAX_ATTRIBS = D_MAX_ATTRIBS;

static uint32 s_lastBoundShader = 0;
static uint32 s_enabledAttribs = 0;

const char * ATTRIB_NAMES[D_MAX_ATTRIBS] = {
	"a_attrib0"
,	"a_attrib1"
,	"a_attrib2"
,	"a_attrib3"
,	"a_attrib4"
,	"a_attrib5"
,	"a_attrib6"
,	"a_attrib7"
};

int32 shaderCompile(uint32 id, const char * pSourceStr);
int32 shaderLink(Shader * pShader, ShaderSource * pVertexShaderSrc, ShaderSource * pFragmentShaderSrc);
void shaderLoadSource(FILE * file, ShaderSource * pShaderSrc);
void shaderFreeSource(ShaderSource * pShaderSrc);

int32 shaderCreate(Shader * pShader, const char * name) {

	int32 result = 0;
	pShader->id = 0;
	pShader->attribs = 0;

	if (name) {
		FILE * vertFile = 0;
		FILE * fragFile = 0;
		char vertFilename[128] = { 0 };
		char fragFilename[128] = { 0 };
		strcpy(vertFilename, SHADERS_DIR);
		strcpy(fragFilename, SHADERS_DIR);
		strcat(vertFilename, name);
		strcat(fragFilename, name);
		strcat(vertFilename, ".vert");
		strcat(fragFilename, ".frag");
		vertFile = fopen(vertFilename, "rb");
		fragFile = fopen(fragFilename, "rb");

		if (vertFile && fragFile) {

			ShaderSource vertSource;
			ShaderSource fragSource;
			shaderLoadSource(vertFile, &vertSource);
			shaderLoadSource(fragFile, &fragSource);
			vertSource.dataLength = 0;
			fragSource.dataLength = 0;

			result = shaderLink(pShader, &vertSource, &fragSource);

			shaderFreeSource(&vertSource);
			shaderFreeSource(&fragSource);
		}

		if (vertFile) {
			fclose(vertFile);
		}

		if (fragFile) {
			fclose(fragFile);
		}
	}

	return result;
}

void shaderLoadSource(FILE * file, ShaderSource * pShaderSrc) {

	fseek(file, 0, SEEK_END);
	pShaderSrc->dataLength = ftell(file);
	fseek(file, 0, SEEK_SET);

	pShaderSrc->dataSource = (uint8 *)malloc(pShaderSrc->dataLength + 1);
	memset(pShaderSrc->dataSource, 0, pShaderSrc->dataLength + 1);

	fread(pShaderSrc->dataSource, 1, pShaderSrc->dataLength, file);
}

void shaderFreeSource(ShaderSource * pShaderSrc) {
	if (pShaderSrc->dataSource) {
		free(pShaderSrc->dataSource);
	}
}

int32 shaderCompile(uint32 id, const char * source) {

	int32 compileStatus = 0;

	glShaderSource(id, 1, &source, 0);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) {

		GLint infoLen = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1) {
			char* infoLog = malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(id, infoLen, NULL, infoLog);
			/// @todo LOG result
			free(infoLog);
		}
	}

	return compileStatus;
}

int32 shaderLink(Shader * pShader, ShaderSource * pVertexShaderSrc, ShaderSource * pFragmentShaderSrc) {

	int32 result = 0;
	uint32 progId;
	int32 compileStatus = 0;
	int32 linkStatus = 0;
	int32 useBinary = 0;
	uint32 vsId;
	uint32 fsId;

	if (!pVertexShaderSrc || !pFragmentShaderSrc
	 || !pVertexShaderSrc->dataSource || !pFragmentShaderSrc->dataSource) {

		return 0;
	}

	if (pVertexShaderSrc->dataLength
	  && pFragmentShaderSrc->dataLength) {
		useBinary = 1;
	}

	vsId = glCreateShader(GL_VERTEX_SHADER);
	fsId = glCreateShader(GL_FRAGMENT_SHADER);

	if (vsId && fsId) {

		if (!useBinary) {

			compileStatus = 1;
			compileStatus &= shaderCompile(vsId, pVertexShaderSrc->dataSource);
			compileStatus &= shaderCompile(fsId, pFragmentShaderSrc->dataSource);

		} else {

			/// create binary here

		}

		if (compileStatus) {

			progId = glCreateProgram();

			if (progId) {

				glAttachShader(progId, vsId);
				glAttachShader(progId, fsId);

				glLinkProgram(progId);

				glGetProgramiv(progId, GL_LINK_STATUS, &linkStatus);

				if (!linkStatus) {

					GLint infoLen = 0;
					glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &infoLen);

					if (infoLen > 1) {

						char* infoLog = malloc(sizeof(char) * infoLen);
						glGetProgramInfoLog(progId, infoLen, NULL, infoLog);
						/// @todo LOG result
						free(infoLog);
					}

					glDeleteProgram(progId);
				} else {

					uint32 i = 0;
					uint32 activeAttribs = 0;

					glGetProgramiv(progId, GL_ACTIVE_ATTRIBUTES, &activeAttribs);

					for (; i < activeAttribs; ++i) {

						glBindAttribLocation(progId, i, ATTRIB_NAMES[i]);
					}

					pShader->id = progId;
					pShader->attribs = activeAttribs;
					result = 1;
				}
			}
		}
	}

	glDeleteShader(fsId);
	glDeleteShader(vsId);

	return result;
}

void shaderBind(Shader * pShader) {

	if (pShader->id != s_lastBoundShader) {

		uint32 attribIndex;
		for (attribIndex = s_enabledAttribs; attribIndex < pShader->attribs; ++attribIndex) {
			glEnableVertexAttribArray(attribIndex);
		}
		for (attribIndex = pShader->attribs; attribIndex < s_enabledAttribs; ++attribIndex) {
			glDisableVertexAttribArray(attribIndex);
		}

		s_lastBoundShader = pShader->id;
		s_enabledAttribs = pShader->attribs;

		glUseProgram(pShader->id);
	}
}

void shaderDestroy(Shader * pShader) {
	glDeleteProgram(pShader->id);
}



