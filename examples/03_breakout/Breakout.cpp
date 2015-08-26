#include "..\..\StdAfx.h"
#include "Breakout.h"
#include "..\..\renderer\DX.h"
#include "..\..\renderer\render_types.h"
#include "..\..\renderer\VIBuffer.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\World.h"

//BaseApp *app = new Breakout();

Breakout::Breakout() {
	_settings.tickCamera = false;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	_settings.clearColor = D3DCOLOR_XRGB(0,0,0);
	//_CrtSetBreakAlloc(339);
}


Breakout::~Breakout() {
	LOG << "removing app now";
	delete _floor_data;
	delete _brick_data;
	delete _ball.data;
	delete _bat.data;
	delete _world;
}


void Breakout::loadContent() {
	_dx.moveCamera(Vector3f(0.0f,0.0f,6.0f));

	int shader = _dx.loadShader("basic","PTCTech");
	// PCT buffer
	BufferDescriptor desc;
	desc.declarationID = 0;
	desc.vertexSize = sizeof(PCTVertex);
	desc.type = 0;
	int id = _dx.createBuffer(desc);

	int tex_id = _dx.loadTexture("array");

	_bat.data = new PCTMeshData(id, 0, 64);
	data::build_cube(_bat.data, 1.0f, 0.2f, 0.2f,Rect(128,0,256,128),512.0f);
	_bat.data->setTextureID(tex_id);

	_brick_data = new PCTMeshData(id, 0, 64);
	data::load_mesh(_brick_data,"brick.mesh",512.0f);
	_brick_data->setTextureID(tex_id);

	_ball.data = new PCTMeshData(id, 0, 64);
	data::build_cube(_ball.data, 0.2f, 0.2f, 0.2f, Rect(256,0,64,64),512.0f);
	_ball.data->setTextureID(tex_id);

	_floor_data = new PCTMeshData(id, 0, 512);
	data::add_grid(_floor_data,1.0f,10,8, Rect(258,66,64,64),512.0f);
	_floor_data->setTextureID(tex_id);
	
	_world = new World(&_dx);
	// paddle
	_bat.position = Vector3f(0.0f, -3.0f, 0.0f);
	_bat.aabBox = AABBox(_bat.position,Vector3f(0.5f,0.1f,0.1f));
	_bat.id = _world->create(_bat.position, _bat.data);	
	// ball
	_ball.position = Vector3f(0.0f,-2.5f,0.0f);
	_ball.id = _world->create(_ball.position,_ball.data);
	_ball.aabBox = AABBox(_ball.position,Vector3f(0.1f,0.1f,0.1f));
	// bricks
	for ( int i = 0; i < 10; ++i ) {
		for ( int j = 0;  j < 8; ++j ) {
			Brick b;
			Vector3f pos = Vector3f(3.5f - i * 0.8f,3.0f - j * 0.4f,0.0f);
			b.id = _world->create(pos,_brick_data,shader);
			b.position = pos;
			b.aabBox = AABBox(pos,Vector3f(0.5f,0.1f,0.1f));
			_bricks.push_back(b);
		}
	}
	_world->create(Vector3f(0.0f,0.0f,-0.6f),_floor_data);

	_timer = 0.0f;
	_ball.sticky = true;
	
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
		_world->translate(_bat.id,_bat.position);
		_bat.aabBox.translate(_bat.position);
		if ( _ball.sticky ) {
			_ball.position = _bat.position;
			_ball.position.y += 0.5f;
			_world->translate(_ball.id,_ball.position);		
			_ball.aabBox.translate(_ball.position);
		}
	}
}

void Breakout::tick(float dt) {
	_timer += dt;
	movePaddle(dt);
	if ( !_ball.sticky ) {
		_ball.position += _ball.velocity * dt;
		bool bounce = false;
		if ( _ball.position.x > 4.0f || _ball.position.x < -4.0f ) {
			bounce = true;
			_ball.velocity.x *= -1.0f;
		}
		if ( _ball.position.y > 3.5f ) {
			bounce = true;
			_ball.velocity.y *= -1.0f;
		}
		if( bounce ) {
			_ball.position += _ball.velocity * dt;
		}
		_ball.aabBox.translate(_ball.position);
		if ( _ball.aabBox.collides(_bat.aabBox)) {
			LOG << "we have a hit - bat : ball";
			_ball.velocity.y *= -1.0f;
			_ball.position += _ball.velocity * dt;
			_ball.aabBox.translate(_ball.position);
		}
		_world->translate(_ball.id,_ball.position);		
		if ( _ball.position.y < -3.5f ) {
			_ball.sticky = true;
			_ball.position = _bat.position;
			_ball.position.y += 0.5f;
			_world->translate(_ball.id,_ball.position);		
			_ball.aabBox.translate(_ball.position);
		}

	}	
	Bricks::iterator it = _bricks.begin();
	while ( it != _bricks.end() ) {
		if ( _ball.aabBox.collides(it->aabBox)) {
			LOG << "we have a hit - brick (" << it->id << ") : ball";
			_ball.velocity.y *= -1.0f;
			_ball.position += _ball.velocity * dt;
			_ball.aabBox.translate(_ball.position);
			_world->remove(it->id);
			it = _bricks.erase(it);
		}
		else {
			++it;
		}
	}
	_world->tick(dt);
}

void Breakout::render() {
	_world->render();
}

void Breakout::onChar(char ascii, unsigned int state) {
	if ( ascii == '1' ) {
		if ( _ball.sticky ) {
			_ball.sticky = false;
			_ball.velocity = Vector3f(2,2,0);
		}
	}
}