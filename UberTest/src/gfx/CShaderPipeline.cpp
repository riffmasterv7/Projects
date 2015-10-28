
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "gles.h"

#include "EShaderType.h"
#include "CShaderPipeline.h"

static const char * SHADERS_DIR = "../data/";

struct SCode
{
	SCode()
		: mData(0)
		, mSize(0)
	{
	}

	~SCode()
	{
		delete [] mData;
	}

	char * mData;
	uint32 mSize;
};


CShaderPipeline::CShaderPipeline(tPipelineId shaderId)
	: mId(0)
	, mActiveAttributes(0)
{
	assemble(shaderId);
}

CShaderPipeline::~CShaderPipeline()
{
	if (mId)
	{
		glDeleteProgram(mId);
	}
}

void CShaderPipeline::assemble(tPipelineId pipelineId)
{
	uint32 shaderIdList[E_SHADER_TYPE_COUNT] = { 0 };
	bool allShadersValid = true;

	mId = glCreateProgram();

	for (int32 i = 0; i < E_SHADER_TYPE_COUNT; ++i)
	{
		char filename[64] = { 0 };

		strcpy(filename, SHADERS_DIR);
		strcat(filename, pipelineId);
		strcat(filename, shaderTypeToFileExtension(i));

		SCode code;

		if (loadCode(filename, code))
		{
			uint32 shaderId = createShader(pipelineId, i, code);

			if (shaderId)
			{
				glAttachShader(mId, shaderId);

				shaderIdList[i] = shaderId;
			}
		}
	}

	glLinkProgram(mId);

	for (int32 i = 0; i < E_SHADER_TYPE_COUNT; ++i)
	{
		if (shaderIdList[i] != 0)
		{
			glDetachShader(mId, shaderIdList[i]);
			glDeleteShader(shaderIdList[i]);
		}
	}

	int32 linkStatus = 0;
	glGetProgramiv(mId, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus) 
	{
		GLint infoLen = 0;
		glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) 
		{
			char* infoLog = new char[infoLen];
			glGetProgramInfoLog(mId, infoLen, NULL, infoLog);

			printf("Pipeline linkage failed (%s) :\n%s\n", pipelineId, infoLog);
			
			delete [] infoLog;
		}

		glDeleteProgram(mId);
		mId = 0;
	} 
	else 
	{
		char ATTRIBUTE_NAME[] = "VS_ATTR_X";
		int32 ATTRIBUTE_NUM_INDEX = strlen(ATTRIBUTE_NAME) - 1;

		int32 activeAttribs = 0;
		glGetProgramiv(mId, GL_ACTIVE_ATTRIBUTES, &activeAttribs);

		for (int32 i = 0; i < activeAttribs; ++i) 
		{
			ATTRIBUTE_NAME[ATTRIBUTE_NUM_INDEX] = i + '0';
			glBindAttribLocation(mId, i, ATTRIBUTE_NAME);
		}

		mActiveAttributes = activeAttribs;
	}
}

bool CShaderPipeline::loadCode(const char * filename, SCode& code)
{
	FILE * file = fopen(filename, "rb");

	bool result = (file != 0);

	if (result)
	{
		fseek(file, 0, SEEK_END);
		code.mSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		code.mData = new char[code.mSize + 1];
		code.mData[code.mSize] = 0;

		fread(code.mData, 1, code.mSize, file);

		fclose(file);
	}

	return result;
}

uint32 CShaderPipeline::createShader(tPipelineId pipelineId, int32 type, SCode& code)
{
	uint32 id = 0;
	int32 compileStatus = 0;

	id = glCreateShader(shaderTypeToGLESType(type));

	glShaderSource(id, 1, &code.mData, 0);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) 
	{
		int32 infoLen = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) 
		{
			char shaderFilename[32] = { 0 };
			strcpy(shaderFilename, pipelineId);
			strcat(shaderFilename, shaderTypeToFileExtension(type));

			char * infoLog = new char[infoLen];
			glGetShaderInfoLog(id, infoLen, NULL, infoLog);
			
			printf("(%s) shader compilation failed (%s) :\n%s\n", shaderTypeToString(type), shaderFilename, infoLog);
			
			delete [] infoLog;
		}

		glDeleteShader(id);
		id = 0;
	}

	return id;
}

uint32 CShaderPipeline::getId() const
{
	return mId;
}

uint32 CShaderPipeline::getActiveAttributes() const
{
	return mActiveAttributes;
}




