#pragma once
#include "..\utils\GameTimer.h"
#include <d3dx9core.h>
// ------------------------------------------
// Basic game settings
// ------------------------------------------
struct BaseSettings {

	int screenSizeX;
	int screenSizeY;
	bool tickCamera;
	D3DXCOLOR clearColor;

	BaseSettings() {
		screenSizeX = 800;
		screenSizeY = 600;
		tickCamera = true;
		clearColor = D3DCOLOR_XRGB(0, 40, 100);
	}
};

// ------------------------------------------
// Base application
// ------------------------------------------
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
	const BaseSettings& getSettings() const {
		return _settings;
	}
protected:
	BaseSettings _settings;
private:
	HWND _handle;
	GameTimer _timer;
	int _ticks;
	float _time;
};

