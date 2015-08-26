// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <dxerr.h>

typedef unsigned int IdString;
typedef unsigned int uint32;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if ( (p) && (p) != NULL) { (p)->Release(); (p)=NULL; } }
#endif

#if defined(RELEASE) 
	#ifndef HR
#define HR(x) { HRESULT hr = x;	if ( FAILED(hr) ) { DXTraceA(__FILE__ ,__LINE__ ,hr,#x,true); 	}} 
	#endif
#else
	#ifndef HR
#define HR(x) { HRESULT hr = x ; if ( FAILED(hr) ) { char desc[1024]; sprintf(desc,"(DX) %s - %s",DXGetErrorString(hr),DXGetErrorDescription(hr));	char buf[2048];	sprintf_s(buf,"%s(%d) : Error: %s\n", __FILE__, __LINE__, desc);LOGE << buf; }}
	#endif
#endif