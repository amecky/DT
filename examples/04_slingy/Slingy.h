#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"
#include <vector>
#include "..\..\particle\ParticleSystem.h"
#include "Grid.h"
#include "Bullets.h"
#include "..\..\renderer\ScreenQuad.h"
#include "..\..\renderer\BloomFilter.h"

class Slingy : public BaseApp {

struct Head {

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

typedef std::vector<Tail> Tails;

public:
	Slingy();
	~Slingy();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
	void onButton(int button, ButtonState state);
private:
	void moveBall(float dt);
	void moveTail(float dt);
	void resetBall();
	void addWall(const v2& p,int width,int height);
	void drawLine(const v2& start,const v2& end,int thickness);
	float _timer;
	Texture _headTexture;
	Texture _tailTexture;
	bool _firing;
	float _fireTimer;
	BulletHandler _bullets;
	Exit _exit;
	v2 _startPos;
	Head _head;
	Tails _tails;
	Grid _grid;
	ParticleSystem* _particles;
	int _rt1;
	ScreenQuad _quad;
	v2 _worldPos;
	BloomFilter* _bloomFilter;
	bool _useBloom;
};

