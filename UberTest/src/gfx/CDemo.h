#ifndef CDEMO
#define CDEMO

#include <types_v7.h>

class CDemo
{
public:

	virtual bool create(void * nativeDisplayType, void * nativeWindowType, uint32 canvasWidth, uint32 canvasHeight) = 0;
	virtual void present() = 0;
	virtual void destroy() = 0;
};

#endif





