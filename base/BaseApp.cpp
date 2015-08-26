#include "..\StdAfx.h"
#include "BaseApp.h"
#include "..\utils\Log.h"
#include "..\utils\Profiler.h"

BaseApp::BaseApp() : _ticks(0) {
}


BaseApp::~BaseApp() {
}


void BaseApp::init(HWND handle) {
	LOGC("BaseApp") << "-> init";
	_handle = handle;
	_dx.init(_handle,_settings.screenSizeX,_settings.screenSizeY);
	profiler::init();
	LOGC("BaseApp") << "--- load content ---";
	loadContent();
	_timer.start();
}

void BaseApp::buildFrame() {
	_timer.tick();
	profiler::reset();
	tick(_timer.getElapsedTime());
	if ( _settings.tickCamera ) {
		_dx.updateCamera();
	}
	_dx.begin(_settings.clearColor);		
	PR_START("RENDERING")
	render();
	PR_END("RENDERING")
	_dx.end();
	++_ticks;
	if ( _ticks > 200 ) {
		profiler::print();
		_ticks = 0;
	}
}

void BaseApp::shutdown() {
	LOGC("BaseApp") << "-> shutdown";
	_dx.shutdown();
}