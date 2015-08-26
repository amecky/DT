#include "..\..\StdAfx.h"
#include "CubeTest.h"
#include "..\..\renderer\DX.h"
#include "..\..\renderer\render_types.h"
#include "..\..\renderer\VIBuffer.h"
#include "..\..\World.h"

BaseApp *app = new CubeTest();

CubeTest::CubeTest() {
}


CubeTest::~CubeTest() {
	//delete _data;
	//delete _world;
}


void CubeTest::loadContent() {
	// PCT buffer
	BufferDescriptor desc;
	desc.declarationID = 0;
	desc.vertexSize = sizeof(PCTVertex);
	desc.type = 0;
	int id = _dx.createBuffer(desc);

	int tex_id = _dx.loadTexture("ref_256");

	_data = new PCTMeshData(id, 0, 64);
	data::build_cube(*_data, 1.0f, 1.0f, 1.0f);
	_data->setTextureID(tex_id);

	_world = new World(&_dx);
	// animated cube
	_id = _world->create(Vector3f(0.0f, 0.0f, 0.0f), _data);	

	_rotating = false;
	_moving = false;
	_scaling = false;
	_timer = 0.0f;

	LOG << "Key mappings:";
	LOG << "'1' - toggle rotation";
	LOG << "'2' - toggle scale";
	LOG << "'3' - toggle movement";
	LOG << "'4' - reset";
}

void CubeTest::tick(float dt) {
	/*
	_timer += dt;
	if ( _rotating ) {
		_world->rotate(_id,Vector3f(0.0f,0.0f,_timer));
	}
	if ( _scaling ) {
		float s = 1.0f + sin(_timer) * 0.2f;
		_world->scale(_id,Vector3f(s,s,s));
	}
	if ( _moving ) {
		float x = 2.0f * cos(_timer);
		float y = 2.0f * sin(_timer);
		_world->translate(_id,Vector3f(x,y,0.0f));
	}
	_world->tick(dt);
	*/
}

void CubeTest::render() {
	_world->render();
}

void CubeTest::onChar(char ascii, unsigned int state) {
	if ( ascii == '1' ) {
		_rotating = !_rotating;
	}
	if ( ascii == '2' ) {
		_scaling = !_scaling;
	}
	if ( ascii == '3' ) {
		_moving = !_moving;
	}
	if ( ascii == '4' ) {
		_rotating = false;
		_scaling = false;
		_moving = false;
		_world->rotate(_id,Vector3f(0,0,0));
		_world->scale(_id,Vector3f(1,1,1));
		_world->translate(_id,Vector3f(0,0,0));
	}
}