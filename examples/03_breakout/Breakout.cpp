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
}

void Breakout::movePaddle(float dt) {
	float dx = 0.0f;
	if ( GetAsyncKeyState('A') != 0 ) {
		dx = 3.0f;
	}
	if ( GetAsyncKeyState('D') != 0 ) {
		dx = -3.0f;
	}
	if ( dx != 0.0f ) {
		_bat.position.x += dx * dt;
		if ( _bat.position.x < -3.9f ) {
			_bat.position.x = -3.9f;
		}
		if ( _bat.position.x > 3.9f ) {
			_bat.position.x = 3.9f;
		}
		//_world->translate(_bat.id,_bat.position);
		_bat.aabBox.translate(_bat.position);
		if ( _ball.sticky ) {
			_ball.position = _bat.position;
			_ball.position.y += 0.5f;
			//_world->translate(_ball.id,_ball.position);		
			_ball.aabBox.translate(_ball.position);
		}
	}
}

void Breakout::tick(float dt) {
	_timer += dt;
}

void Breakout::render() {
	sprites::begin();
	for ( int x = 0; x < 10; ++x ) {
		for ( int y = 0; y < 6; ++y ) {
			sprites::draw(v2(40 + x * 80,370 + y * 40),_brickTexture);	
		}
	}
	sprites::end();
}

void Breakout::onChar(char ascii, unsigned int state) {
	if ( ascii == '1' ) {
		if ( _ball.sticky ) {
			_ball.sticky = false;
			_ball.velocity = Vector3f(2,2,0);
		}
	}
}