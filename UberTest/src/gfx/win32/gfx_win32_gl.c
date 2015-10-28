
#include "../gfx.h"

#if defined(D_GFX_OPENGL) && defined(D_OS_FAMILY_WIN32)

int32 gfxCreateContext() {

	PIXELFORMATDESCRIPTOR pfd;
	int32 pixelFormat;
	int32 errorCode;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags	 = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cRedBits = g_GFX_CONFIG.colorRedBits;
	pfd.cBlueBits = g_GFX_CONFIG.colorBlueBits;
	pfd.cGreenBits = g_GFX_CONFIG.colorGreenBits;
	pfd.cAlphaBits = g_GFX_CONFIG.colorAlphaBits;
	pfd.cDepthBits = g_GFX_CONFIG.depthBits;
	pfd.cStencilBits = g_GFX_CONFIG.stencilBits;

	g_GFX_MAIN_DATA.dcHandle = GetDC(g_GFX_MAIN_DATA.wndHandle);
	pixelFormat = ChoosePixelFormat(g_GFX_MAIN_DATA.dcHandle, &pfd);
	errorCode = NO_ERROR;

	if (pixelFormat != 0) {
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat(g_GFX_MAIN_DATA.dcHandle, pixelFormat, sizeof(pfd), &bestMatch_pfd);

		// TODO: Double-check  the closet match pfd for anything unacceptable...

		if (bestMatch_pfd.cDepthBits < pfd.cDepthBits) {
			// POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
			return 0;
		}

		if (SetPixelFormat(g_GFX_MAIN_DATA.dcHandle, pixelFormat, &pfd) == 0) {
			errorCode = GetLastError();
			// TODO: Report cause of failure here...
			return 0;
		}
	} else {
		errorCode = GetLastError();
		// TODO: Report cause of failure here...
		return 0;
	}

	g_GFX_MAIN_DATA.apiData.glrcHandle = wglCreateContext(g_GFX_MAIN_DATA.dcHandle);
	wglMakeCurrent(g_GFX_MAIN_DATA.dcHandle, g_GFX_MAIN_DATA.apiData.glrcHandle);

	return 1;
}

void gfxReleaseContext() {

	if (g_GFX_MAIN_DATA.apiData.glrcHandle != 0) {

		wglMakeCurrent(0, 0);
		wglDeleteContext(g_GFX_MAIN_DATA.apiData.glrcHandle);
		g_GFX_MAIN_DATA.apiData.glrcHandle = 0;
	}

	if (g_GFX_MAIN_DATA.dcHandle != 0) {

		ReleaseDC(g_GFX_MAIN_DATA.wndHandle, g_GFX_MAIN_DATA.dcHandle);
		g_GFX_MAIN_DATA.dcHandle = 0;
	}
}

void gfxSwapBuffers() {
	SwapBuffers(g_GFX_MAIN_DATA.dcHandle);
}

#endif

