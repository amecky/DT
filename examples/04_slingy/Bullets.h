#pragma once
#include "..\..\renderer\render_types.h"

const int MAX_TRAILS = 256;

struct TrailArray {

	v2* position;
	v2* scale;
	float* rotation;
	Color* color;
	float* timer;
	char* buffer;

	int count;
	int countAlive;

	TrailArray() : count(0), countAlive(0), buffer(0) {
		initialize();
	}

	~TrailArray() {
		if (buffer != 0) {
			delete[] buffer;
		}
	}

	void initialize() {
		int size = MAX_TRAILS * (sizeof(v2) * 2 + sizeof(float) + sizeof(Color) + sizeof(float));
		buffer = new char[size];
		position = (v2*)(buffer);
		scale = (v2*)(position + MAX_TRAILS);
		rotation = (float*)(scale + MAX_TRAILS);
		color = (Color*)(rotation + MAX_TRAILS);
		timer = (float*)(color + MAX_TRAILS);
		count = MAX_TRAILS;
		countAlive = 0;
	}

	void swapData(int a, int b) {
		if (a != b) {
			position[a] = position[b];
			scale[a] = scale[b];
			rotation[a] = rotation[b];
			color[a] = color[b];
			timer[a] = timer[b];
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
const int MAX_BULLETS = 64;

struct BulletArray {

	v2* position;
	v2* previous;
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
		int size = MAX_BULLETS * (sizeof(v2) * 4 + sizeof(float) + sizeof(Color));
		buffer = new char[size];
		position = (v2*)(buffer);
		previous = (v2*)(position + MAX_BULLETS);
		velocity = (v2*)(previous + MAX_BULLETS);
		scale = (v2*)(velocity + MAX_BULLETS);
		rotation = (float*)(scale + MAX_BULLETS);
		color = (Color*)(rotation + MAX_BULLETS);
		count = MAX_BULLETS;
		countAlive = 0;
	}

	void swapData(int a, int b) {
		if (a != b) {
			position[a] = position[b];
			previous[a] = previous[b];
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


class BulletHandler {

public:
	BulletHandler();
	~BulletHandler() {}
	void tick(float dt);
	void render();
	void start(const v2& pos,float angle);
private:
	Texture _bulletTexture;
	Texture _trailTexture;
	BulletArray _bullets;
	TrailArray _trails;
};
