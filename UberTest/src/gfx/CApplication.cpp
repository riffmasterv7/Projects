
#include "CApplication.h"
#include "CDemo.h"

#include "CWindowWINAPI.h"

/*
#include "CWindowX11.h"
#include "CWindowSCREEN.h"
#include "CWindowKD.h"
#include "CWindowWAL2D.h"
#include "CWindowIVI.h"
*/

struct SApplicationPrivate
{
	SApplicationPrivate()
		: mWindow(0)
	{

	}

	CWindow * mWindow;
};

CApplication::CApplication()
	: mPrivate(*new SApplicationPrivate)
{
}

CApplication::~CApplication()
{
	delete &mPrivate;
}

bool CApplication::initialize(int32 agrc, const char ** argv)
{
	SWindowSettings settings("GLES 3.1 Demo: XXX", 0, 0, 1280, 800);

	mPrivate.mWindow = new CWindowWINAPI(settings);

	return true;
}

void CApplication::run(CDemo * demo)
{
	if (mPrivate.mWindow->create())
	{
		if (demo->create(mPrivate.mWindow->getNativeDisplayType(), mPrivate.mWindow->getNativeWindowType(), mPrivate.mWindow->getSettings().mWidth, mPrivate.mWindow->getSettings().mHeight))
		{
			do 
			{
				mPrivate.mWindow->update();

				demo->present();
			} 
			while (!mPrivate.mWindow->getSignals()->mTerminate);

			demo->destroy();
		}

		mPrivate.mWindow->destroy();
	}
}

void CApplication::shutdown()
{
	delete mPrivate.mWindow;
}
