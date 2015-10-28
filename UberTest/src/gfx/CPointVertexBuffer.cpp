
#include "gles.h"

#include "CPointVertexBuffer.h"

CPointVertexBuffer::CPointVertexBuffer(void * data, uint32 size)
	: CVertexBuffer(data, size)
{

}

CPointVertexBuffer::~CPointVertexBuffer()
{

}

void CPointVertexBuffer::setup() const
{
	//glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(SPointVertex), (const void *)offsetof(SPointVertex, position));
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
}