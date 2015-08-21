#include "GameTimer.h"
#include "Log.h"

GameTimer::GameTimer(void) {
	QueryPerformanceFrequency(&_frequency); 
	LOG << "frequency: " << _frequency.QuadPart;
	QueryPerformanceCounter(&_lastTime);
	_timerStopped = true;
	_numFrames = 0;
	_totalTime = 0.0;
}


GameTimer::~GameTimer(void) {
}

void GameTimer::start() {
	if ( _timerStopped ) {
		QueryPerformanceCounter(&_lastTime ); 
		_timerStopped = false;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Stops the timer. 
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void GameTimer::stop() {
	if ( !_timerStopped ) {
		LARGE_INTEGER stopTime; 
		QueryPerformanceCounter(&stopTime); 
		double d = (double)stopTime.QuadPart;//
		double l = (double)_lastTime.QuadPart;
		double t = (d - l) * 1000000.0f;
		t /= _frequency.QuadPart;
		_elapsed = t;
		//_runningTime += (float)d / (float)_ticksPerSecond; 
		_timerStopped = true;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Summary: Updates the timer. Calculates the time elapsed since the last Update call. 
Updates the frames per second and updates the total running time. 
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
void GameTimer::tick() {
	if ( !_timerStopped ) {
		LARGE_INTEGER stopTime; 
		QueryPerformanceCounter(&stopTime); 
		double d = (double)stopTime.QuadPart;//
		double l = (double)_lastTime.QuadPart;
		double t = (d - l) * 1000000.0f;
		t /= _frequency.QuadPart;
		t /= 1000.0f;
		t /= 1000.0f;
		_elapsed = t;
		_totalTime += t;
		_lastTime = stopTime;
		++_numFrames;

	}	
}