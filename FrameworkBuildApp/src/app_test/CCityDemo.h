#ifndef CCITYDEMO
#define CCITYDEMO

#include <CDemo.h>

#include <CGraphicsContext.h>

class CCityDemo : public CDemo
{
public:
	CCityDemo();
	virtual ~CCityDemo();

	virtual bool create(void * nativeDisplayId, void * nativeWindowId, uint32 canvasWidth, uint32 canvasHeight);
	virtual void present();
	virtual void destroy();

private:

	CGraphicsContext mGraphicsContext;

};

#endif





