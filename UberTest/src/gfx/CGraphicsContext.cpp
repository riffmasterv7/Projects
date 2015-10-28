#include "CGraphicsContext.h"

#include "egl.h"

#include <stdio.h>

struct SGraphicsContextPrivate
{
	SGraphicsContextPrivate();

	EGLDisplay mDisplay;
	EGLContext mContext;
	EGLSurface mSurface;
	EGLConfig  mConfig;
};

#undef D_TABLE_ENTRY
#define D_EGL_CFG_ATTR_TABLE \
	D_TABLE_ENTRY(EGL_BUFFER_SIZE) \
	D_TABLE_ENTRY(EGL_ALPHA_SIZE) \
	D_TABLE_ENTRY(EGL_BLUE_SIZE) \
	D_TABLE_ENTRY(EGL_GREEN_SIZE) \
	D_TABLE_ENTRY(EGL_RED_SIZE) \
	D_TABLE_ENTRY(EGL_DEPTH_SIZE) \
	D_TABLE_ENTRY(EGL_STENCIL_SIZE) \
	D_TABLE_ENTRY(EGL_CONFIG_CAVEAT) \
	D_TABLE_ENTRY(EGL_CONFIG_ID) \
	D_TABLE_ENTRY(EGL_LEVEL) \
	D_TABLE_ENTRY(EGL_MAX_PBUFFER_HEIGHT) \
	D_TABLE_ENTRY(EGL_MAX_PBUFFER_PIXELS) \
	D_TABLE_ENTRY(EGL_MAX_PBUFFER_WIDTH) \
	D_TABLE_ENTRY(EGL_NATIVE_RENDERABLE) \
	D_TABLE_ENTRY(EGL_NATIVE_VISUAL_ID) \
	D_TABLE_ENTRY(EGL_NATIVE_VISUAL_TYPE) \
	D_TABLE_ENTRY(EGL_SAMPLES) \
	D_TABLE_ENTRY(EGL_SAMPLE_BUFFERS) \
	D_TABLE_ENTRY(EGL_SURFACE_TYPE) \
	D_TABLE_ENTRY(EGL_TRANSPARENT_TYPE) \
	D_TABLE_ENTRY(EGL_TRANSPARENT_BLUE_VALUE) \
	D_TABLE_ENTRY(EGL_TRANSPARENT_GREEN_VALUE) \
	D_TABLE_ENTRY(EGL_TRANSPARENT_RED_VALUE) \
	D_TABLE_ENTRY(EGL_NONE) \
	D_TABLE_ENTRY(EGL_BIND_TO_TEXTURE_RGB) \
	D_TABLE_ENTRY(EGL_BIND_TO_TEXTURE_RGBA) \
	D_TABLE_ENTRY(EGL_MIN_SWAP_INTERVAL) \
	D_TABLE_ENTRY(EGL_MAX_SWAP_INTERVAL) \
	D_TABLE_ENTRY(EGL_LUMINANCE_SIZE) \
	D_TABLE_ENTRY(EGL_ALPHA_MASK_SIZE) \
	D_TABLE_ENTRY(EGL_COLOR_BUFFER_TYPE) \
	D_TABLE_ENTRY(EGL_RENDERABLE_TYPE) \
	D_TABLE_ENTRY(EGL_MATCH_NATIVE_PIXMAP) \
	D_TABLE_ENTRY(EGL_CONFORMANT)


#define D_MAX_EGL_CONFIGS 4


SGraphicsContextPrivate::SGraphicsContextPrivate()
	: mDisplay(0)
	, mContext(0)
	, mSurface(0)
	, mConfig(0)
{
}

CGraphicsContext::CGraphicsContext()
	: mPrivate(*new SGraphicsContextPrivate)
{
}

CGraphicsContext::~CGraphicsContext()
{
	delete &mPrivate;
}


bool CGraphicsContext::createDisplay(void * nativeDisplayType, void * nativeWindowType, CGraphicsContext& context)
{
	EGLint eglError;
	EGLint minorVersion;
	EGLint majorVersion;
	EGLint numConfigs = 0;
	EGLDisplay eglDisplay;
	EGLSurface eglSurface;
	EGLConfig eglChoosenConfig;
	EGLConfig eglConfigs[D_MAX_EGL_CONFIGS] = {0};

	EGLint surfaceAttribList[] = 
	{
		EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
		EGL_NONE
	};
	
	EGLint configAttribList[] = 
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
		EGL_LEVEL,			0,
		EGL_RED_SIZE,		8,
		EGL_GREEN_SIZE,		8,
		EGL_BLUE_SIZE,		8,
		EGL_ALPHA_SIZE,		8,
		EGL_DEPTH_SIZE,		24,
		EGL_STENCIL_SIZE,	8,
		EGL_SAMPLE_BUFFERS, 1,
		EGL_SAMPLES, 4,
		EGL_CONFIG_CAVEAT, EGL_NONE,
		EGL_NONE
	};

	//////////////////////////////////////////////////////////////////////////

	eglDisplay = eglGetDisplay((EGLNativeDisplayType)nativeDisplayType);

	if (eglDisplay == EGL_NO_DISPLAY) 
	{
		/// Report an error!
		return false;
	}

	if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion)) 
	{
		/// Report an error!
		return false;
	}

	if (!eglChooseConfig(eglDisplay, configAttribList, eglConfigs, D_MAX_EGL_CONFIGS, &numConfigs)) 
	{
		/// Report an error!
		return false;
	}

	if (numConfigs == 0) 
	{
		/// Report an error!
		return 0;
	}

	if (0)
	{
		EGLint cfgIdx = 0;
		EGLint value = 0;

		while (cfgIdx < numConfigs) 
		{

			EGLint attrib = EGL_BUFFER_SIZE;
			while (attrib != (EGL_CONFORMANT + 1)) 
			{

				eglGetConfigAttrib(eglDisplay, eglConfigs[cfgIdx], attrib, &value);

				switch (attrib) 
				{

#define D_TABLE_ENTRY(entry) case (entry): printf("%32s", #entry); printf(" - %d  %x\n", value, value); break;
					D_EGL_CFG_ATTR_TABLE
#undef D_TABLE_ENTRY

default :
	break;
				}

				++attrib;
			}

			printf("\n\n");

			++cfgIdx;
		}
	}

	eglChoosenConfig = eglConfigs[0];

	eglSurface = eglCreateWindowSurface(eglDisplay, eglChoosenConfig, (EGLNativeWindowType)nativeWindowType, 0);

	eglError = eglGetError();
	if (eglSurface == EGL_NO_SURFACE || (eglError != EGL_SUCCESS)) 
	{
		switch (eglError) {
		case EGL_BAD_MATCH:
			/// Report an error!
			break;
		case EGL_BAD_ALLOC:
			/// Report an error!
			break;
		case EGL_BAD_CONFIG:
			/// Report an error!
			break;
		case EGL_BAD_NATIVE_WINDOW:
			/// Report an error!
			break;
		default:
			break;
		}

		return false;
	}

	context.mPrivate.mDisplay = eglDisplay;
	context.mPrivate.mSurface = eglSurface;
	context.mPrivate.mConfig = eglChoosenConfig;

	return true;
}

bool CGraphicsContext::createContext(CGraphicsContext& context, const CGraphicsContext * sharedContext)
{
	EGLint contextAttribList[] = 
	{
		EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
		EGL_CONTEXT_MINOR_VERSION_KHR, 1,
		EGL_NONE
	};

	EGLContext eglContext = eglCreateContext(context.mPrivate.mDisplay, context.mPrivate.mConfig, sharedContext ? sharedContext->mPrivate.mContext : 0, contextAttribList);

	if (eglContext == EGL_NO_CONTEXT) 
	{
		EGLint eglError = eglGetError();
		switch (eglError) 
		{
			case EGL_BAD_CONFIG:
				/// Report an error!
				break;
			default:
				/// Report an error!
				break;
		}

		return false;
	}

	if (!eglBindAPI(EGL_OPENGL_ES_API)) 
	{
		/// Report an error!
		return false;
	}

	context.mPrivate.mContext = eglContext;

	if (sharedContext)
	{
		context.mPrivate.mDisplay = sharedContext->mPrivate.mDisplay;
		context.mPrivate.mSurface = sharedContext->mPrivate.mSurface;
		context.mPrivate.mConfig  = sharedContext->mPrivate.mConfig;
	}

	return true;

}

void CGraphicsContext::destroyContext(CGraphicsContext& context)
{
	eglDestroyContext(context.mPrivate.mDisplay, context.mPrivate.mContext);

	context.mPrivate.mContext = EGL_NO_CONTEXT;
}

void CGraphicsContext::destroyDisplay(CGraphicsContext& context)
{
	eglDestroySurface(context.mPrivate.mDisplay, context.mPrivate.mSurface);
	eglTerminate(context.mPrivate.mDisplay);	

	context.mPrivate.mDisplay = EGL_NO_DISPLAY;
	context.mPrivate.mSurface = EGL_NO_SURFACE;
	context.mPrivate.mConfig = 0;
}

void CGraphicsContext::activate(CGraphicsContext& context)
{
	eglMakeCurrent(context.mPrivate.mDisplay, context.mPrivate.mSurface, context.mPrivate.mSurface, context.mPrivate.mContext);
}

void CGraphicsContext::present(CGraphicsContext& context)
{
	eglSwapBuffers(context.mPrivate.mDisplay, context.mPrivate.mSurface);
}


/*
int32 gfxCreateContext() 
{
	const char * extensions = (char *)glGetString(GL_EXTENSIONS);
	int32 len = strlen(extensions);

	char * extFixed = new char[len + 1];
	strcpy(extFixed, extensions);
	extFixed[len] = 0;

	char * currChar = extFixed;
	while (*currChar)
	{
		if (*currChar == ' ')
		{
			*currChar = '\n';
		}

		++currChar;
	}

	printf("%s\n", extFixed);

	return 1;
}
*/