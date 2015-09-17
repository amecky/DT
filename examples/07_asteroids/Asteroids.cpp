#include "..\..\StdAfx.h"
#include "Asteroids.h"
#include "..\..\renderer\render_types.h"
#include "..\..\World.h"
#include "..\..\utils\Log.h"
#include "..\..\renderer\gfx.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\renderer\Shader.h"
#include "..\..\sprites\SpriteBatch.h"
#include "..\..\math\mathutils.h"

BaseApp *app = new Asteroids();

Asteroids::Asteroids() {
	_settings.tickCamera = false;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	_settings.clearColor = Color(0,0,0);
}

Asteroids::~Asteroids() {
}

void Asteroids::loadContent() {
	gfx::getCamera()->createOrthoProjectionMatrix(1024,768);
	sprites::intialize("asteroids");	
	_asteroidSettings.angularSpeed = 0.4f;
	_asteroidSettings.radiusSpeed = 2.0f;
	_asteroidSettings.velocity = 70.0f;
	_asteroidSettings.velocityVariance = 20.0f;
	createAsteroid(0);
}

void Asteroids::createAsteroid(int type) {
	Asteroid a;
	a.set(_asteroidSettings);
	float x = 100 + math::random(0,800);
	float y = 100 + math::random(0,600);
	a.create(type,v2(x,y));
	_asteroids.push_back(a);
}

void Asteroids::tick(float dt) {
	for ( size_t i = 0; i < _asteroids.size(); ++i ) {
		_asteroids[i].tick(dt);
	}
	AsteroidList::iterator it = _asteroids.begin();
	while ( it != _asteroids.end()) {
		if ( it->isDead() ) {
			it = _asteroids.erase(it);
		}
		else {
			++it;
		}
	}
}

void Asteroids::render() {
	sprites::begin();
	for ( size_t i = 0; i < _asteroids.size(); ++i ) {
		_asteroids[i].render();
	}
	sprites::end();
}

void Asteroids::onChar(char ascii, unsigned int state) {
	if ( ascii == 'a' ) {
		_asteroids[0].hit();
	}
	if ( ascii == '1' ) {
		createAsteroid(0);
	}
	if ( ascii == '2' ) {
		createAsteroid(1);
	}
	if ( ascii == '3' ) {
		createAsteroid(2);
	}
	if ( ascii == '4' ) {
		createAsteroid(3);
	}
}