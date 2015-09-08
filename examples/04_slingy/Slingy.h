#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"
#include <vector>
#include "..\..\particle\ParticleSystem.h"
#include "Grid.h"

const int MAX_BULLETS = 64;

struct BulletArray {

	v2* position;
	v2* velocity;
	v2* scale;
	float* rotation;
	Color* color;
	char* buffer;

	int count;
	int countAlive;

	BulletArray() : count(0), countAlive(0), buffer(0) {
		initialize();
	}

	~BulletArray() {
		if (buffer != 0) {
			delete[] buffer;
		}
	}

	void initialize() {
		int size = MAX_BULLETS * (sizeof(v2) * 3 + sizeof(float) + sizeof(Color));
		buffer = new char[size];
		position = (v2*)(buffer);
		velocity = (v2*)(position + MAX_BULLETS);
		scale = (v2*)(velocity + MAX_BULLETS);
		rotation = (float*)(scale + MAX_BULLETS);
		color = (Color*)(rotation + MAX_BULLETS);
		count = MAX_BULLETS;
		countAlive = 0;
	}

	void swapData(int a, int b) {
		if (a != b) {
			position[a] = position[b];
			velocity[a] = velocity[b];
			scale[a] = scale[b];
			rotation[a] = rotation[b];
			color[a] = color[b];
		}
	}

	void kill(int id) {
		if (countAlive > 0) {
			swapData(id, countAlive - 1);
			--countAlive;
		}
	}

	void wake(int id) {
		if (countAlive < count)	{
			swapData(id, countAlive);
			++countAlive;
		}
	}

};

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
	Texture _bulletTexture;
	BulletArray _bullets;
	Exit _exit;
	v2 _startPos;
	Head _head;
	Tails _tails;
	Grid _grid;
	ParticleSystem* _particles;
};

