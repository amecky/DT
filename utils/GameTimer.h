#pragma once
#include "..\stdafx.h"

class GameTimer {

public:
	GameTimer(void);
	~GameTimer(void);
	void start();
    void stop();
    void tick(); 

    bool isStopped() { 
		return _timerStopped; 
	}
    float getElapsedTime() { 
		return _timerStopped ? 0.0f : _elapsed; 
	}
	double getTotalTime() const {
		return _totalTime;
	}
private:
	LARGE_INTEGER _frequency;
    LARGE_INTEGER _lastTime;
	LARGE_INTEGER _lastFPSTime;
	double _totalTime;
	int _numFrames;
    float _elapsed;
    bool _timerStopped;
};

