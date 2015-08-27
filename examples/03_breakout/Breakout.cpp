#include "..\..\StdAfx.h"
#include "Breakout.h"
#include "..\..\renderer\render_types.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\sprites\SpriteBatch.h"
#include "..\..\utils\Log.h"

BaseApp *app = new Breakout();

Breakout::Breakout() {
	_settings.tickCamera = false;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	//_settings.clearColor = D3DCOLOR_XRGB(0,0,0);
	//_CrtSetBreakAlloc(339);
}


Breakout::~Breakout() {
}


void Breakout::loadContent() {
	sprites::intialize("content\\array.png");
	_brickTexture = sprites::buildTexture(Rect(350,0,70,30),512.0f,512.0f,true);
	_bat.position = v2(400,60);
	_bat.aabBox = AABBox(_bat.position,v2(45,15));
	_ball.position = v2(400,120);
	_ball.aabBox = AABBox(_ball.position,v2(10,10));
	for ( int x = 0; x < 10; ++x ) {
		for ( int y = 0; y < 6; ++y ) {
			Brick b;
			b.position = v2(40 + x * 80,370 + y * 40);
			b.aabBox = AABBox(b.position,v2(35,15));
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
		LOG << "collision";
		_ball.velocity.y *= -1.0f;
		_ball.position += _ball.velocity * dt;
	}
	for ( size_t i = 0; i < _bricks.size(); ++i ) {
		Brick& b = _bricks[i];
		if ( _ball.aabBox.collides(b.aabBox)) {
			_ball.velocity.y *= -1.0f;
			_ball.position += _ball.velocity * dt;
		}		
	}
}

void Breakout::render() {
	sprites::begin();
	for ( size_t i = 0; i < _bricks.size(); ++i ) {
		sprites::draw(_bricks[i].position,_brickTexture);	
	}
	sprites::draw(_ball.position,sprites::buildTexture(Rect(382,2,20,20),512.0f,512.0f,true));
	sprites::draw(_bat.position,sprites::buildTexture(Rect(350,80,90,30),512.0f,512.0f,true));
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