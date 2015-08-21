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
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow) {
	LOG << "Registering class";
	ATOM ret = MyRegisterClass(hInstance);
	LOG << "Returned: " << ret;
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
	}
	app->init(_handle);
	/*
	// PC buffer
	BufferDescriptor de;
	de.declarationID = 0;
	de.vertexSize = sizeof(PCVertex);
	de.type = 1;
	int pcid = dx.createBuffer(de);

	// PCT buffer
	BufferDescriptor desc;
	desc.declarationID = 0;
	desc.vertexSize = sizeof(PCTVertex);
	desc.type = 0;
	int id = dx.createBuffer(desc);

	int tid = dx.loadTexture("case");
	int blid = dx.loadTexture("blurred");
	
	PCTMeshData data(id,0,64);
	data::build_cube(data,1.0f,1.0f,1.0f);
	data.setTextureID(tid);

	PCTMeshData floor(id,0,64);
	floor.setTextureID(blid);
	//data::build_cube(floor,12.0f,0.1f,12.0f);
	data::add_xz_plane(floor,12.0f,12.0f);
	data::add_line(floor,Vector3f(4.0f,0.0f,0.0f),Vector3f(-4.0f,2.0f,0.0f),0.4f);


	World w(&dx);
	// create floor
	w.create(Vector3f(0.0f,-3.0f,0.0f),&floor);
	// create three cubes
	//w.create(Vector3f(0.0f,0.0f,0.0f),&data);
	//w.create(Vector3f(2.0f,0.0f,0.0f),&data);
	//w.create(Vector3f(-2.0f,0.0f,0.0f),&data);

	// animated cube
	MID mi = w.create(Vector3f(0.0f,-2.0f,0.0f),&data);
	//w.rotate(mi,Vector3f(0.0f,0.0f,D3DX_PI/4.0f));
	//w.scale(mi,Vector3f(2.0f,2.0f,2.0f));


	BezierCurve curve;
	curve.p0 = Vector3f(8.0f,2.0f,0.0f);
	curve.p1 = Vector3f(2.0f,6.0f,0.0f);
	curve.p2 = Vector3f(-2.0f,-4.0f,0.0f);
	curve.p3 = Vector3f(-8.0f,2.0f,0.0f);
	curve.build();
	*/
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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance; // Store instance handle in our global variable
   _handle = CreateWindowEx(0,"ClassName", "Test", WS_OVERLAPPEDWINDOW,300,300,800,600, NULL, NULL, hInstance, NULL);
   if (!_handle) {
	   LOGE << "Cannot create window";
      return FALSE;
   }
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
