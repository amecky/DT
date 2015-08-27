#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"
#include <vector>

class Breakout : public BaseApp {

struct Brick {
	v2 position;
	AABBox aabBox;
};

struct Bat {
	v2 position;
	AABBox aabBox;
};

struct Ball {

	bool sticky;
	v2 position;
	v2 velocity;
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
	void moveBall(float dt);
	float _timer;
	Texture _brickTexture;
	Bricks _bricks;
	Ball _ball;
	Bat _bat;
};

