#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "Asteroid.h"
#include <vector>
#include "Ship.h"

class Asteroids : public BaseApp {

typedef std::vector<Asteroid> AsteroidList;

public:
	Asteroids();
	~Asteroids();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
private:
	void createAsteroid(int type);
	void drawGrid(const v3& pos);
	AsteroidList _asteroids;
	AsteroidSettings _asteroidSettings;
	D3DXMATRIX _world;
	float _rotation;
	Ship _ship;
	Texture _gridTex;
};

