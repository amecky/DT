#include "..\StdAfx.h"
#include "BaseApp.h"
#include "..\utils\Log.h"
#include "..\utils\Profiler.h"
#include "..\renderer\gfx.h"

BaseApp::BaseApp() : _ticks(0) , _time(0.0f) , _ticking(true) {	
}


BaseApp::~BaseApp() {
}


void BaseApp::init(HWND handle) {
	LOGC("BaseApp") << "-> init";
	_handle = handle;
	//_dx.init(_handle,_settings.screenSizeX,_settings.screenSizeY);
	gfx::initialize(_settings.screenSizeX,_settings.screenSizeY,true,handle,false,1000.0f,0.1f);
	profiler::init();
	LOGC("BaseApp") << "--- load content ---";
	loadContent();
	_timer.start();
}

void BaseApp::setMousePos(int x, int y) {
	gfx::setMousePos(x, y);
}

void BaseApp::buildFrame() {
	_timer.tick();
	profiler::reset();
	debug::reset();
	PR_START("FRAME")
	if ( _ticking ) {
		tick(_timer.getElapsedTime());
		if ( _settings.tickCamera ) {
			//_dx.updateCamera();
			gfx::getCamera()->tick();
			gfx::getCamera()->Update();
		}
	}
	gfx::beginRendering(_settings.clearColor);
	PR_START("RENDERING")
	render();
	PR_END("RENDERING")
	gfx::endRendering();
	//++_ticks;
	//_time += _timer.getElapsedTime();
	PR_END("FRAME")
	/*
	if ( _time >= 1.0f ) {
		_time -= 1.0f;
		LOG << "ticks: " << _ticks;
		profiler::print();
		debug::log();
		_ticks = 0;
	}
	*/
}

void BaseApp::shutdown() {
	LOGC("BaseApp") << "-> shutdown";
	//_dx.shutdown();
	gfx::shutdown();
}