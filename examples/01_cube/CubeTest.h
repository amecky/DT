#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"

class CubeTest : public BaseApp {

public:
	CubeTest();
	~CubeTest();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	bool _rotating;
	bool _moving;
	bool _scaling;
	float _timer;
};

