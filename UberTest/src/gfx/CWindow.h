#ifndef CWINDOW
#define CWINDOW

#include <types_v7.h>

struct SWindowSettings
{
	SWindowSettings();
	SWindowSettings(const char * title, int32 posX, int32 posY, int32 width, int32 height);

	SWindowSettings(const SWindowSettings& other);
	SWindowSettings& operator= (const SWindowSettings& other);

	~SWindowSettings();

	char * mTitle;
	uint32 mPosX;
	uint32 mPosY;
	uint32 mWidth;
	uint32 mHeight;
};

struct SWindowSignals
{
	SWindowSignals();

	bool mTerminate;
};

class CWindow
{
public:
	CWindow(SWindowSettings& settings);

	virtual ~CWindow();

	virtual void * getNativeDisplayType() const = 0;
	virtual void * getNativeWindowType() const = 0;

	virtual const SWindowSignals * getSignals() const;
	
	const SWindowSettings&  getSettings() const;

	virtual bool create() = 0;
	virtual void update() = 0;
	virtual void destroy() = 0;

protected:

	SWindowSettings mSettings;
	SWindowSignals mSignals;
};

#endif
