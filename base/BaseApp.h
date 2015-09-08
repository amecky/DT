#pragma once
#include "..\utils\GameTimer.h"
//#include <d3dx9core.h>
#include "..\renderer\render_types.h"

enum ButtonState {
	BS_UP,BS_DOWN,BS_COUNT
};
// ------------------------------------------
// Basic game settings
// ------------------------------------------
struct BaseSettings {

	int screenSizeX;
	int screenSizeY;
	bool tickCamera;
	Color clearColor;

	BaseSettings() {
		screenSizeX = 800;
		screenSizeY = 600;
		tickCamera = true;
		clearColor = Color(0, 40, 100);
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
	void setMousePos(int x, int y);
	virtual void onButton(int button, ButtonState state) {}
protected:
	BaseSettings _settings;
private:
	HWND _handle;
	GameTimer _timer;
	int _ticks;
	float _time;
};

