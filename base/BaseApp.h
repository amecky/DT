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
	v3 cameraPosition;

	BaseSettings() {
		screenSizeX = 800;
		screenSizeY = 600;
		tickCamera = true;
		clearColor = Color(0, 40, 100);
		cameraPosition = v3(0.0f, 0.0f, -5.0f);
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
	void toggleTicking() {
		_ticking = !_ticking;
	}
protected:
	BaseSettings _settings;
private:
	bool _ticking;
	HWND _handle;
	GameTimer _timer;
	int _ticks;
	float _time;
};

