#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"

class Breakout : public BaseApp {

struct Brick {
	MID id;
	Vector3f position;
	AABBox aabBox;
};

struct Bat {
	MID id;
	PCTMeshData* data;
	Vector3f position;
	AABBox aabBox;
};

struct Ball {

	bool sticky;
	Vector3f position;
	PCTMeshData* data;
	MID id;
	Vector3f velocity;
	AABBox aabBox;

};

typedef std::vector<Brick> Bricks;

public:
	Breakout();
	~Breakout();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	void movePaddle(float dt);
	World* _world;
	PCTMeshData* _brick_data;
	PCTMeshData* _floor_data;
	float _timer;
	Bricks _bricks;
	Ball _ball;
	Bat _bat;
};

