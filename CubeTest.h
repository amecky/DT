#pragma once
#include "base\BaseApp.h"
#include "renderer\Mesh.h"

class World;

class CubeTest : public BaseApp {

public:
	CubeTest();
	~CubeTest();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	World* _world;
	PCTMeshData* _floor;
	PCTMeshData* _data;
};

