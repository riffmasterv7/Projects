#ifndef CWINDOWWINAPI
#define CWINDOWWINAPI

#include "CWindow.h"

struct SWindowWINAPIPrivate;

class CWindowWINAPI : public CWindow
{
public:
	CWindowWINAPI(SWindowSettings& settings);

	virtual ~CWindowWINAPI();

	void * getNativeDisplayType() const;
	void * getNativeWindowType() const;

	virtual bool create(); 
	virtual void update(); 
	virtual void destroy();

private:

	SWindowWINAPIPrivate& mPrivate;
};

#endif
