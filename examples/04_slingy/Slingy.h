#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"
#include <vector>

class Slingy : public BaseApp {

struct Ball {

	bool sticky;
	v2 position;
	v2 velocity;
	AABBox aabBox;
	float angle;
};

struct Tail {

	v2 position;
	float angle;
	AABBox aabBox;

};

struct Wall {
	v2 position;
	AABBox aabBox;
	Texture texture;
};

typedef std::vector<Tail> Tails;
typedef std::vector<Wall> Walls;

public:
	Slingy();
	~Slingy();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	void moveBall(float dt);
	void moveTail(float dt);
	void resetBall();
	void addWall(const v2& p,int width,int height);
	float _timer;
	Texture _ballTexture;
	Texture _tailTexture;
	Ball _ball;
	Tails _tails;
	Walls _walls;
};

