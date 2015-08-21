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
	_dx.init(_handle);
	profiler::init();
	LOGC("BaseApp") << "--- load content ---";
	loadContent();
	_timer.start();
}

void BaseApp::buildFrame() {
	_timer.tick();
	profiler::reset();
	tick(_timer.getElapsedTime());
	_dx.begin();		
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