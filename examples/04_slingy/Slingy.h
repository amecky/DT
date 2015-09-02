#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"
#include <vector>
#include "..\..\particle\ParticleSystem.h"

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

struct Exit {
	v2 position;
	Texture texture;
	AABBox aabBox;
};

struct Star {
	v2 position;
	Texture texture;
	AABBox aabBox;
};

struct Wall {
	v2 position;
	AABBox aabBox;
	Texture texture;
};

typedef std::vector<Tail> Tails;
typedef std::vector<Wall> Walls;
typedef std::vector<Star> Stars;

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
	void drawLine(const v2& start,const v2& end,int thickness);
	float _timer;
	Texture _ballTexture;
	Texture _tailTexture;
	Exit _exit;
	Stars _stars;
	v2 _startPos;
	Ball _ball;
	Tails _tails;
	Walls _walls;

	ParticleSystem* _particles;
	ParticleEmitter* _emitter;
};

