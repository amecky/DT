#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"

class CubeMeshTest : public BaseApp {

public:
	CubeMeshTest();
	~CubeMeshTest();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	World* _world;
	PCTMeshData* _data;
	MID _id;
	bool _rotating;
	bool _moving;
	bool _scaling;
	float _timer;
};

