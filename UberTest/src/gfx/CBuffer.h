
#ifndef _CBUFFER_
#define _CBUFFER_

#include <types_v7.h>

enum EBufferType 
{
	E_VERTEX_BUFFER,
	E_INDEX_BUFFER,
	E_SHADER_BUFFER,
	E_ATOMIC_BUFFER,

	E_BUFFER_TYPE_COUNT
};

const char* bufferTypeToString(int32 type);
const int32 bufferTypeToGLESType(int32 type);

class CBuffer
{

protected:

	CBuffer(int32 type);
	~CBuffer();
	
	void allocate(const void * data, uint32 size);

protected:

	int32 mTarget;
	uint32 mId;
};

class CVertexBuffer : public CBuffer
{
public:
	CVertexBuffer(void * data, uint32 size);
	~CVertexBuffer() {};

	static void bind(const CVertexBuffer * buffer);

	virtual void setup() const = 0;
};

class CIndexBuffer : public CBuffer
{
public:
	CIndexBuffer(void * data, uint32 size);
	~CIndexBuffer() {};

	static void bind(const CIndexBuffer * buffer);

	virtual void setup() const {};
};


class CVertexSetup
{
public :
	CVertexSetup();
	~CVertexSetup();

	void init(CVertexBuffer * vertex, CIndexBuffer * index) const;

	static void bind(const CVertexSetup * setup);
	
private:
	uint32 mId;
};

#endif