#include "..\..\StdAfx.h"
#include "Breakout.h"
#include "..\..\renderer\render_types.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\sprites\SpriteBatch.h"
#include "..\..\utils\Log.h"
#include "..\..\math\mathutils.h"
//BaseApp *app = new Breakout();

Breakout::Breakout() {
	_settings.tickCamera = false;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	_settings.clearColor = Color(0,0,0);
	//_CrtSetBreakAlloc(339);
}


Breakout::~Breakout() {
	sprites::shutdown();
}


void Breakout::loadContent() {
	sprites::intialize("content\\array.png");
	_brickTexture = math::buildTexture(Rect(60,0,60,25),512.0f,512.0f,true);
	_bat.position = v2(400,60);
	_bat.aabBox = AABBox(_bat.position,v2(45,15));
	_ball.position = v2(400,120);
	_ball.aabBox = AABBox(_ball.position,v2(10,10));
	for ( int x = 0; x < 12; ++x ) {
		for ( int y = 0; y < 6; ++y ) {
			Brick b;
			b.position = v2(40 + x * 65,370 + y * 30);
			b.aabBox = AABBox(b.position,v2(30,12));
			_bricks.push_back(b);
		}
	}
}

void Breakout::movePaddle(float dt) {
	float dx = 0.0f;
	if ( GetAsyncKeyState('A') != 0 ) {
		dx = -300.0f;
	}
	if ( GetAsyncKeyState('D') != 0 ) {
		dx = 300.0f;
	}
	if ( dx != 0.0f ) {
		_bat.position.x += dx * dt;
		if ( _bat.position.x < 50.0f ) {
			_bat.position.x = 50.0f;
		}
		if ( _bat.position.x > 750.0f ) {
			_bat.position.x = 750.0f;
		}
		_bat.aabBox.translate(_bat.position);
		if ( _ball.sticky ) {
			_ball.position = _bat.position;
			_ball.position.y += 50.0f;
			_ball.aabBox.translate(_ball.position);
		}
	}
}

void Breakout::moveBall(float dt) {
	_ball.position += _ball.velocity * dt;
	if ( _ball.position.x < 10.0f || _ball.position.x > 790.0f ) {
		_ball.velocity.x *= -1.0f;
		_ball.position += _ball.velocity * dt;
	}
	if ( _ball.position.y < 10.0f || _ball.position.y > 590.0f ) {
		_ball.velocity.y *= -1.0f;
		_ball.position += _ball.velocity * dt;
	}
	_ball.aabBox.translate(_ball.position);
}

void Breakout::tick(float dt) {
	_timer += dt;
	movePaddle(dt);
	moveBall(dt);
	if ( _ball.aabBox.collides(_bat.aabBox) ) {
		LOG << "collision - bat and ball";
		_ball.velocity.y *= -1.0f;
		_ball.position += _ball.velocity * dt;
	}
	Bricks::iterator it = _bricks.begin();
	while ( it != _bricks.end()) {		
		if ( _ball.aabBox.collides(it->aabBox)) {
			LOG << "collision ball and btick";
			_ball.velocity.y *= -1.0f;
			_ball.position += _ball.velocity * dt;
			it = _bricks.erase(it);
		}		
		else {
			++it;
		}
	}
}

void Breakout::render() {
	sprites::begin();
	for ( size_t i = 0; i < _bricks.size(); ++i ) {
		sprites::draw(_bricks[i].position,_brickTexture);	
	}
	sprites::draw(_ball.position,math::buildTexture(Rect(30,50,24,24),512.0f,512.0f,true));
	sprites::draw(_bat.position,math::buildTexture(Rect(0,80,90,30),512.0f,512.0f,true));
	sprites::end();
}

void Breakout::onChar(char ascii, unsigned int state) {
	if ( ascii == '1' ) {
		if ( _ball.sticky ) {
			_ball.sticky = false;
			_ball.velocity = v2(250,250);
		}
	}
}