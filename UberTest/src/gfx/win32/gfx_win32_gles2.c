
#include "../gfx.h"

#if defined (D_GFX_OPENGL_ES_2) && defined(D_OS_FAMILY_WIN32)

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


#define D_WAIT_CLIENT
#undef D_WAIT_CLIENT

#define D_MAX_EGL_CONFIGS 4

int32 gfxCreateContext() {

	EGLint eglError;
	EGLint minorVersion;
	EGLint majorVersion;
	EGLint numConfigs = 0;
	EGLDisplay eglDisplay;
	EGLSurface eglSurface;
	EGLContext eglContext;
	EGLConfig eglChoosenConfig;
	EGLConfig eglConfigs[D_MAX_EGL_CONFIGS] = {0};

	EGLint contextAttribList[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLint surfaceAttribList[] = {
		EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
		EGL_NONE
	};

	EGLint configAttribList[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_LEVEL, 0,
		EGL_RED_SIZE, g_GFX_CONFIG.colorRedBits,
		EGL_GREEN_SIZE, g_GFX_CONFIG.colorGreenBits,
		EGL_BLUE_SIZE, g_GFX_CONFIG.colorBlueBits,
		EGL_ALPHA_SIZE, g_GFX_CONFIG.colorAlphaBits,
		EGL_DEPTH_SIZE, g_GFX_CONFIG.depthBits,
		EGL_STENCIL_SIZE, g_GFX_CONFIG.stencilBits,
		EGL_SAMPLE_BUFFERS, 1,
		EGL_SAMPLES, 4,
		EGL_CONFIG_CAVEAT, EGL_NONE,
		EGL_NONE
	};

	//////////////////////////////////////////////////////////////////////////

	eglDisplay = eglGetDisplay(g_GFX_MAIN_DATA.dcHandle);
	if (eglDisplay == EGL_NO_DISPLAY) {
		/// Report an error!
		return 0;
	}

	if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion)) {
		/// Report an error!
		return 0;
	}

	if (!eglChooseConfig(eglDisplay, configAttribList, eglConfigs, D_MAX_EGL_CONFIGS, &numConfigs)) {
		/// Report an error!
		return 0;
	}

	if (numConfigs == 0) {
		/// Report an error!
		return 0;
	}

	{
		EGLint cfgIdx = 0;
		EGLint value = 0;

		while (cfgIdx < numConfigs) {

			EGLint attrib = EGL_BUFFER_SIZE;
			while (attrib != (EGL_CONFORMANT + 1)) {

				eglGetConfigAttrib(eglDisplay, eglConfigs[cfgIdx], attrib, &value);

				switch (attrib) {

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

	eglSurface = eglCreateWindowSurface(eglDisplay, eglChoosenConfig, g_GFX_MAIN_DATA.wndHandle, 0);
	eglError = eglGetError();
	if (eglSurface == EGL_NO_SURFACE || (eglError != EGL_SUCCESS)) {
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

		return 0;
	}

	eglContext = eglCreateContext(eglDisplay, eglChoosenConfig, EGL_NO_CONTEXT, contextAttribList);
	if (eglContext == EGL_NO_CONTEXT) {
		EGLint eglError = eglGetError();
		switch (eglError) {
			case EGL_BAD_CONFIG:
				/// Report an error!
				break;
			default:
				/// Report an error!
				break;
		}

		return 0;
	}

// 	if (!eglBindAPI(EGL_OPENGL_ES_API)) {
// 		/// Report an error!
// 		return 0;
// 	}

	if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
		/// Report an error!
		return 0;
	}

	g_GFX_MAIN_DATA.apiData.eglContext = eglContext;
	g_GFX_MAIN_DATA.apiData.eglSurface = eglSurface;
	g_GFX_MAIN_DATA.apiData.eglDisplay = eglDisplay;

	//eglSwapInterval(eglDisplay, 60);

	return 1;
}

void gfxReleaseContext() {

	eglDestroyContext(g_GFX_MAIN_DATA.apiData.eglDisplay, g_GFX_MAIN_DATA.apiData.eglContext);
	eglDestroySurface(g_GFX_MAIN_DATA.apiData.eglDisplay, g_GFX_MAIN_DATA.apiData.eglSurface);
	eglTerminate(g_GFX_MAIN_DATA.apiData.eglDisplay);
}

void gfxSwapBuffers() {

#if defined(D_WAIT_CLIENT)

	EGLBoolean res;
	res = eglWaitClient();

	if (res == EGL_FALSE) {
		/// Post warning
	}
#endif

	eglSwapBuffers(g_GFX_MAIN_DATA.apiData.eglDisplay, g_GFX_MAIN_DATA.apiData.eglSurface);
}

#endif