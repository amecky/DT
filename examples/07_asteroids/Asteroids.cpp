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
#include "..\..\renderer\BatchRenderer.h"

BaseApp *app = new Asteroids();

Asteroids::Asteroids() {
	_settings.tickCamera = false;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	_settings.clearColor = Color(0,0,0);
	_settings.cameraPosition = v3(0.0f, -2.5f, -5.0f);
}

Asteroids::~Asteroids() {
	batch::shutdown();
}

void Asteroids::loadContent() {
	//gfx::getCamera()->createOrthoProjectionMatrix(1024,768);
	//sprites::intialize("asteroids");	
	batch::intialize("asteroids");	
	_asteroidSettings.angularSpeed = 0.4f;
	_asteroidSettings.radiusSpeed = 2.0f;
	_asteroidSettings.velocity = 70.0f;
	_asteroidSettings.velocityVariance = 0.2f;
	//createAsteroid(0);
	_rotation = 0.0f;
	_gridTex = math::buildTexture(0.0f, 60.0f, 30.0f, 30.0f, 512.0f, 512.0f, false);
}

void Asteroids::createAsteroid(int type) {
	Asteroid a;
	a.set(_asteroidSettings);
	float x = math::random(-2.0f,2.0f);
	float y = math::random(-1.2f,1.2f);
	a.create(type,v3(x,y,0.0f));
	_asteroids.push_back(a);
}

void Asteroids::tick(float dt) {
	if (GetAsyncKeyState('D') != 0) {
		_rotation -= D3DX_PI * 0.4f * dt;
		_ship.rotate(D3DX_PI * 0.4f * dt);
	}
	if (GetAsyncKeyState('A') != 0) {
		_rotation += D3DX_PI * 0.4f * dt;
		_ship.rotate(-D3DX_PI * 0.4f * dt);
	}
	if (GetAsyncKeyState('W') != 0) {
		v2 v = math::get_radial_velocity(_rotation - DEGTORAD(90.0f), 0.4f) * dt;
		_ship.move(v3(v, 0.0f));
	}

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

void Asteroids::drawGrid(const v3& pos) {
	v3 p[4];
	float d = 0.5f;
	p[0] = v3(pos.x - d, pos.y - d, 0.1f);
	p[1] = v3(pos.x - d, pos.y + d, 0.1f);
	p[2] = v3(pos.x + d, pos.y + d, 0.1f);
	p[3] = v3(pos.x + d, pos.y - d, 0.1f);
	batch::drawQuad(p, _gridTex);
}

void Asteroids::render() {
	batch::begin();
	D3DXMATRIX t;
	D3DXMatrixIdentity(&t);
	D3DXMATRIX rz;
	D3DXMatrixRotationZ(&rz, _rotation);
	_world = rz * t;
	batch::setWorldMatrix(_world);
	for (int y = 0; y < 5; ++y) {
		for (int i = 0; i < 6; ++i) {
			drawGrid(v3(-2.5f + i, -2.0f + y, 0.0f));
		}
	}
	//sprites::begin();
	for ( size_t i = 0; i < _asteroids.size(); ++i ) {
		_asteroids[i].render();
	}
	//sprites::end();
	_ship.render();
	batch::end();
}

void Asteroids::onChar(char ascii, unsigned int state) {
	if (ascii == 't') {
		toggleTicking();
	}
	if (ascii == 'z') {
		LOG << "rotation: " << RADTODEG(_rotation);
	}
	if (ascii == 'r') {
		gfx::getCamera()->setPosition(0.0f, 0.0f, -5.0f);
		_rotation = 0.0f;
		gfx::getCamera()->Update();
	}
	//if ( ascii == 'a' ) {
		//_asteroids[0].hit();
	//}
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