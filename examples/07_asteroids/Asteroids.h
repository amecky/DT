#pragma once
#include "..\..\base\BaseApp.h"
#include "..\..\renderer\Mesh.h"
#include "Asteroid.h"
#include <vector>

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
	AsteroidList _asteroids;
	AsteroidSettings _asteroidSettings;
};

