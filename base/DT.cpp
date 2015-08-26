// DT.cpp : Defines the entry point for the application.
//

#include "..\stdafx.h"
#include "BaseApp.h"
#include "..\renderer\DX.h"
#include "..\utils\Log.h"
#include "..\renderer\VIBuffer.h"
#include "..\utils\GameTimer.h"
#include "..\renderer\Mesh.h"
#include "..\utils\Profiler.h"
#include "..\World.h"
#include "..\math\BezierCurve.h"
#include "..\particle\ParticleSystem.h"
#include "..\particle\ParticleModifier.h"

#pragma comment (lib, "d3d9.lib")

#define MAX_LOADSTRING 100

extern BaseApp* app;

HINSTANCE hInst;
HWND _handle;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int,const BaseSettings&);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow) {

#ifdef _DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
	//flag |= _CRTDBG_DELAY_FREE_MEM_DF;
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	LOG << "Registering class";
	ATOM ret = MyRegisterClass(hInstance);
	LOG << "Returned: " << ret;
	// Perform application initialization:
	const BaseSettings& settings = app->getSettings();
	if (!InitInstance (hInstance, nCmdShow,settings)) {
		return FALSE;
	}
	app->init(_handle);
	
	bool running = true;
	MSG msg;
	while ( running ) {
		// Main message loop:
		while (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if ( msg.message == WM_QUIT) {
			running = false;
		}
		app->buildFrame();		
	}
	app->shutdown();	
	delete app;
	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wc;
	wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "ClassName";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	return RegisterClassEx(&wc);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow,const BaseSettings& settings) {
   hInst = hInstance; // Store instance handle in our global variable
   RECT DesktopSize;	
   GetClientRect(GetDesktopWindow(),&DesktopSize);
   _handle = CreateWindowEx(0,"ClassName", "Test", WS_OVERLAPPEDWINDOW,(DesktopSize.right - settings.screenSizeX) / 2,(DesktopSize.bottom - settings.screenSizeY) / 2,settings.screenSizeX,settings.screenSizeY, NULL, NULL, hInstance, NULL);
   if (!_handle) {
	   LOGE << "Cannot create window";
      return FALSE;
   }
   RECT rect = { 0, 0, settings.screenSizeX, settings.screenSizeY};	
   AdjustWindowRect( &rect, GetWindowLong( _handle, GWL_STYLE ), FALSE );	
   SetWindowPos( _handle, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 
		SWP_NOZORDER | SWP_NOMOVE  );
   ShowWindow(_handle, nCmdShow);
   UpdateWindow(_handle);
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CHAR: {
			char ascii = wParam;
			unsigned int keyState = lParam;
			app->onChar(ascii,keyState);
		}			
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);	
}
