#include "..\StdAfx.h"
#include "BaseApp.h"
#include "..\utils\Log.h"
#include "..\utils\Profiler.h"

BaseApp::BaseApp() : _ticks(0) , _time(0.0f) {
}


BaseApp::~BaseApp() {
}


void BaseApp::init(HWND handle) {
	LOGC("BaseApp") << "-> init";
	_handle = handle;
	//_dx.init(_handle,_settings.screenSizeX,_settings.screenSizeY);
	_dx.initialize(_settings.screenSizeX, _settings.screenSizeY, true, handle, false, 10000.0f, 0.100000000015f);
	profiler::init();
	LOGC("BaseApp") << "--- load content ---";
	loadContent();
	_timer.start();
}

void BaseApp::buildFrame() {
	_timer.tick();
	profiler::reset();
	PR_START("FRAME")
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
	_time += _timer.getElapsedTime();
	PR_END("FRAME")
	if ( _time >= 1.0f ) {
		_time -= 1.0f;
		LOG << "ticks: " << _ticks;
		profiler::print();
		_ticks = 0;
	}
}

void BaseApp::shutdown() {
	LOGC("BaseApp") << "-> shutdown";
	_dx.shutdown();
}