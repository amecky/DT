#pragma once
#include "..\renderer\DX.h"
#include "..\utils\GameTimer.h"

class BaseApp {

public:
	BaseApp(void);
	virtual ~BaseApp(void);
	void init(HWND handle);
	void buildFrame();
	void shutdown();

	virtual void loadContent() = 0;
	virtual void tick(float dt) = 0;
	virtual void render() = 0;

	virtual void onChar(char ascii,unsigned int state) {}
protected:
	DX _dx;	
private:
	HWND _handle;
	GameTimer _timer;
	int _ticks;
};

