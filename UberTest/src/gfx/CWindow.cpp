#include "CWindow.h"

#include <string.h>

namespace
{
	void copyString(char*& dest, const char * src)
	{
		if (src)
		{
			int32 len = strlen(src);

			if (len)
			{
				dest = new char[len + 1];
				strcpy(dest, src);
				dest[len] = 0;
			}
		}
	}
}

SWindowSettings::SWindowSettings()
:	mTitle(0)
,   mPosX(0)
,	mPosY(0)
,	mWidth(0)
,	mHeight(0)
{

}

SWindowSettings::SWindowSettings(const char * title, int32 posX, int32 posY, int32 width, int32 height)
:	mPosX(posX)
,	mPosY(posY)
,	mWidth(width)
,	mHeight(height)
{
	copyString(mTitle, title);
}

SWindowSettings::SWindowSettings(const SWindowSettings& other)
:	mPosX(other.mPosX)
,	mPosY(other.mPosY)
,	mWidth(other.mWidth)
,	mHeight(other.mHeight)
{
	copyString(mTitle, other.mTitle);
}

SWindowSettings& SWindowSettings::operator= (const SWindowSettings& other)
{
	return SWindowSettings(other);
}

SWindowSettings::~SWindowSettings()
{
	delete mTitle;
}

//////////////////////////////////////////////////////////////////////////

SWindowSignals::SWindowSignals()
	: mTerminate(false)
{

}

//////////////////////////////////////////////////////////////////////////


CWindow::CWindow(SWindowSettings& settings)
	: mSettings(settings)
	, mSignals()
{

}

CWindow::~CWindow()
{

}

const SWindowSignals * CWindow::getSignals() const
{
	return &mSignals;
}

const SWindowSettings& CWindow::getSettings() const
{
	return mSettings;
}