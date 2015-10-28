#ifndef __GFX__
#define __GFX__

#include <types_v7.h>

typedef struct _GFX_MAIN_DATA	GFX_MAIN_DATA;
typedef struct _GFX_API_DATA	GFX_API_DATA;
typedef struct _GFX_CONFIG		GFX_CONFIG;

#if defined(WIN32)

	#include <windows.h>

	#define D_OS_FAMILY_WIN32
	#define D_GFX_OPENGL_ES_2
	//#define D_GFX_OPENGL_ES_2_NV
	//#define D_GFX_OPENGL
	//#define D_GFX_DX9
	//#define D_GFX_DX10
	//#define D_GFX_DX11
	
#else
	#error "Supported only windows OS"
#endif


//////////////////////////////////////////////////////////////////////////
/// Graphics api configuration

#if defined (D_GFX_OPENGL)

	#if defined(D_OS_FAMILY_WIN32)

		#include <gl/glew.h>
		#include <gl/gl.h>
		#pragma comment(lib, "../../Redist/lib/ext/gl/glew32.lib")
		#pragma comment(lib, "../../Redist/lib/ext/gl/opengl32.lib")

//////////////////////////////////////////////////////////////////////////
		struct _GFX_API_DATA {
			HGLRC glrcHandle;
		};
//////////////////////////////////////////////////////////////////////////

	#else
		#error 
	#endif

#elif defined (D_GFX_OPENGL_ES_2) || defined (D_GFX_OPENGL_ES_2_NV)

	#if defined (D_OS_FAMILY_WIN32)

		#if defined (D_GFX_OPENGL_ES_2) && !defined (D_GFX_OPENGL_ES_2_NV)

			#include <GLES2/gl2.h>
			#include <EGL/egl.h>
			#pragma comment(lib, "../../Redist/lib/ext/gles2/libEGL.lib")
			#pragma comment(lib, "../../Redist/lib/ext/gles2/libGLESv2.lib")
		#else
			#include <GLES2/gl2.h>
			#include <EGL/egl.h>
			#pragma comment(lib, "../../Redist/lib/ext/GLES2/NV/libGLES20.lib")
		#endif

//////////////////////////////////////////////////////////////////////////
		struct _GFX_API_DATA {
			EGLDisplay eglDisplay;
			EGLSurface eglSurface;
			EGLContext eglContext;
		};
//////////////////////////////////////////////////////////////////////////

	#else //!defined(OS_FAMILY_WIN32)
		#error
	#endif //defined(OS_FAMILY_WIN32)

#else
	#error "Graphics api not specified"
#endif


//////////////////////////////////////////////////////////////////////////
/// Application platform data

#if defined (D_OS_FAMILY_WIN32)

	struct _GFX_MAIN_DATA {
		GFX_API_DATA apiData;
		HINSTANCE instHandle;
		HDC dcHandle;
		HWND wndHandle;
		uint16 width;
		uint16 height;
		uint8 fullscreen;
		uint8 initialized;
		uint8 quit;
	};

#else
	#error Application has not any other platform support except windows
#endif

//////////////////////////////////////////////////////////////////////////

struct _GFX_CONFIG {

	float pixelSizeX;
	float pixelSizeY;

	uint16 width;
	uint16 height;

	uint8 colorRedBits;
	uint8 colorGreenBits;
	uint8 colorBlueBits;
	uint8 colorAlphaBits;

	uint8 depthBits;
	uint8 stencilBits;
	uint8 fullscreen;
	uint8 frameSkip;

	uint8 frameRate;

	uint8 reserved[3];
};

//////////////////////////////////////////////////////////////////////////

extern GFX_MAIN_DATA g_GFX_MAIN_DATA;
extern GFX_CONFIG g_GFX_CONFIG;

int32 gfxCreateContext();
void gfxReleaseContext();
void gfxSwapBuffers();

#endif