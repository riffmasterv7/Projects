

#include <windows.h>

#include "CWindowWINAPI.h"

struct SWindowWINAPIPrivate
{
	SWindowWINAPIPrivate(SWindowSignals * signals)
		: mSignals(signals)
	{

	}

	SWindowSignals * mSignals;

	HINSTANCE mInstance;
	HWND mNativeWindowType;
	HDC  mNativeDisplayType;

	static LRESULT CALLBACK s_WinProc(HWND hWnd, uint32 uiMsg, WPARAM wParam, LPARAM lParam);

	LRESULT WinProc(HWND hWnd, uint32 uiMsg, WPARAM wParam, LPARAM lParam);
};


LRESULT CALLBACK SWindowWINAPIPrivate::s_WinProc(HWND hWnd, uint32 uiMsg, WPARAM wParam, LPARAM lParam)
{
	SWindowWINAPIPrivate * pPrivate = 0;

	if (uiMsg == WM_NCCREATE) 
	{
		// Recover the "this" pointer which was passed as a parameter
		// to CreateWindow(Ex).
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pPrivate = static_cast<SWindowWINAPIPrivate*>(lpcs->lpCreateParams);

		// Put the value in a safe place for future use
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pPrivate));
	} 
	else 
	{
		// Recover the "this" pointer from where our WM_NCCREATE handler
		// stashed it.
		pPrivate = reinterpret_cast<SWindowWINAPIPrivate*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pPrivate) 
	{
		// Now that we have recovered our "this" pointer, let the
		// member function finish the job.
		return pPrivate->WinProc(hWnd, uiMsg, wParam, lParam);
	}

	// We don't know what our "this" pointer is, so just do the default
	// thing. Hopefully, we didn't need to customize the behavior yet.
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

LRESULT SWindowWINAPIPrivate::WinProc(HWND hWnd, uint32 uiMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uiMsg) {

	case WM_ACTIVATEAPP:
		break;

	case WM_KEYDOWN:

		switch (wParam) {

		case VK_ESCAPE:
			mSignals->mTerminate = true;
			break;
		default:
			break;
		}
		break;

	case WM_SIZE:
		break;
	case WM_DESTROY:
		mSignals->mTerminate = true;
		break;

	case WM_POWERBROADCAST:
	case WM_GETMINMAXINFO:
	case WM_SYSCOMMAND:
		if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) {
			return 0;
		}
	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);

	}

	return 0;
}








//////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////


CWindowWINAPI::CWindowWINAPI(SWindowSettings& settings)
	: CWindow(settings)
	, mPrivate(*new SWindowWINAPIPrivate(&mSignals))
{

}

CWindowWINAPI::~CWindowWINAPI()
{
	delete &mPrivate;
}

void * CWindowWINAPI::getNativeDisplayType() const
{
	return (void *)mPrivate.mNativeDisplayType;
}

void * CWindowWINAPI::getNativeWindowType() const
{
	return (void *)mPrivate.mNativeWindowType;
}

bool CWindowWINAPI::create()
{
	bool result = false;

	mPrivate.mInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;

	memset(&wc, 0, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = SWindowWINAPIPrivate::s_WinProc; 
	wc.cbClsExtra = 0; 
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_CLASSDC;
	wc.cbWndExtra = 0; 
	wc.hInstance = mPrivate.mInstance; 
	wc.hIcon = LoadIcon((HINSTANCE) NULL, IDI_APPLICATION); 
	wc.hCursor = LoadCursor((HINSTANCE) NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wc.lpszMenuName =  NULL; 
	wc.lpszClassName = mSettings.mTitle; 

	if (RegisterClassEx(&wc)) 
	{
		int32 w = mSettings.mWidth;
		int32 h = mSettings.mHeight;
		w += GetSystemMetrics(SM_CXFRAME)*2;
		h += GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYFRAME)*2;

		mPrivate.mNativeWindowType = CreateWindowExA(0, mSettings.mTitle, mSettings.mTitle, WS_OVERLAPPEDWINDOW
			, CW_USEDEFAULT, CW_USEDEFAULT, w, h
			, (HWND) NULL, (HMENU) NULL, mPrivate.mInstance, (LPVOID)&mPrivate); 

		if (mPrivate.mNativeWindowType)
		{
			mPrivate.mNativeDisplayType = GetDC(mPrivate.mNativeWindowType);

			ShowWindow(mPrivate.mNativeWindowType, SW_SHOW); 
			UpdateWindow(mPrivate.mNativeWindowType);

			result = true;
		}
	}

	return result;
}

void CWindowWINAPI::update()
{
	MSG msg;

	if (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) 
	{

		if (GetMessage(&msg, 0, 0, 0)) 
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}

		if (mSignals.mTerminate) 
		{
			PostMessage(mPrivate.mNativeWindowType, WM_CLOSE, 0, 0);
		}
	}
}

void CWindowWINAPI::destroy()
{
	ShowWindow(mPrivate.mNativeWindowType, SW_HIDE);

	DestroyWindow(mPrivate.mNativeWindowType);

	UnregisterClass(mSettings.mTitle, mPrivate.mInstance);
}





