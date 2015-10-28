
#include "gles.h"

#include "CBuffer.h"

const char* bufferTypeToString(int32 type)
{
	static const char * bufferTypesText[E_BUFFER_TYPE_COUNT] =
	{
		"VERTEX",
		"INDEX",
		"SHADER",
		"ATOMIC"
	};

	return bufferTypesText[type];
}

const int32 bufferTypeToGLESType(int32 type)
{
	static int32 bufferTypesGLES[E_BUFFER_TYPE_COUNT] =
	{
		GL_ARRAY_BUFFER,
		GL_ELEMENT_ARRAY_BUFFER,
		GL_SHADER_STORAGE_BUFFER,
		GL_ATOMIC_COUNTER_BUFFER
	};

	return bufferTypesGLES[type];
}

//////////////////////////////////////////////////////////////////////////

CBuffer::CBuffer(int32 type)
	: mTarget(bufferTypeToGLESType(type))
{
	glGenBuffers(1, &mId);
}
CBuffer::~CBuffer()
{
	glDeleteBuffers(1, &mId);
}

void CBuffer::allocate(const void * data, uint32 size)
{
	glBufferData(mTarget, size, data, GL_STATIC_DRAW);
}

//////////////////////////////////////////////////////////////////////////

CVertexBuffer::CVertexBuffer(void * data, uint32 size)
	: CBuffer(E_VERTEX_BUFFER)
{
	CVertexBuffer::bind(this);
	allocate(data, size);
	CVertexBuffer::bind(0);
}

void CVertexBuffer::bind(const CVertexBuffer * buffer)
{
	if (buffer)
	{
		glBindBuffer(buffer->mTarget, buffer->mId);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//////////////////////////////////////////////////////////////////////////

CIndexBuffer::CIndexBuffer(void * data, uint32 size)
	: CBuffer(E_INDEX_BUFFER)
{
	CIndexBuffer::bind(this);
	allocate(data, size);
	CIndexBuffer::bind(0);
}


void CIndexBuffer::bind(const CIndexBuffer * buffer)
{
	if (buffer)
	{
		glBindBuffer(buffer->mTarget, buffer->mId);
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

//////////////////////////////////////////////////////////////////////////

CVertexSetup::CVertexSetup()
{
	glGenVertexArrays(1, &mId);
}

CVertexSetup::~CVertexSetup()
{
	glDeleteVertexArrays(1, &mId);
}

void CVertexSetup::init(CVertexBuffer * vertex, CIndexBuffer * index) const
{
	CVertexSetup::bind(this);
	int32 error = glGetError();

	if (vertex)
	{
		CVertexBuffer::bind(vertex);
		vertex->setup();
	}
	else
	{
		CVertexBuffer::bind(0);
	}

	if (index)
	{
		CIndexBuffer::bind(index);
		index->setup();
	}
	else
	{
		CIndexBuffer::bind(0);
	}


	CVertexSetup::bind(0);
}

void CVertexSetup::bind(const CVertexSetup * setup)
{
	glBindVertexArray(setup ? setup->mId : 0);
}
