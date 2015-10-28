
#ifndef _CPOINTBUFFER_
#define _CPOINTBUFFER_

#include "CBuffer.h"

struct SPointVertex
{
	float position[4];
};

class CPointVertexBuffer : public CVertexBuffer
{
public:
	CPointVertexBuffer(void * data, uint32 size);
	~CPointVertexBuffer();

	virtual void setup() const;
};

#endif