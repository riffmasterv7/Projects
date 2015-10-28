
#ifndef CSHADERPIPELINE
#define CSHADERPIPELINE

#include <types_v7.h>

typedef const char * tPipelineId;

struct SCode;

class CShaderPipeline
{
public:

	CShaderPipeline(tPipelineId pipelineId);
	~CShaderPipeline();

	uint32 getId() const;
	uint32 getActiveAttributes() const;

private:
		
	bool loadCode(const char * filename, SCode& code);
	uint32 createShader(tPipelineId pipelineId, int32 type, SCode& code);

	void assemble(tPipelineId pipelineId);

private:

	uint32 mId;
	uint32 mActiveAttributes;
};

#endif