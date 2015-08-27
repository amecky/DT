#include "..\..\StdAfx.h"
#include "CubeMeshTest.h"
#include "..\..\renderer\render_types.h"
#include "..\..\World.h"
#include "..\..\utils\JSONReader.h"

//BaseApp *app = new CubeMeshTest();

CubeMeshTest::CubeMeshTest() {
	_settings.tickCamera = true;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	_settings.clearColor = D3DCOLOR_XRGB(0,0,0);
}


CubeMeshTest::~CubeMeshTest() {
	delete _data;
	delete _world;
}


void CubeMeshTest::loadContent() {
	/*
	BufferDescriptor desc;
	desc.declarationID = 0;
	desc.vertexSize = sizeof(PCTVertex);
	desc.type = 0;
	int id = _dx.createBuffer(desc);

	int tex_id = _dx.loadTexture("ref_256");

	_data = new PCTMeshData(id, 0, 64);
	_data->setTextureID(tex_id);

	_world = new World(&_dx);
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

	data::load_mesh(_data,"test.mesh",256.0f);	
	*/
}

void CubeMeshTest::tick(float dt) {
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
}

void CubeMeshTest::render() {
	_world->render();
}

void CubeMeshTest::onChar(char ascii, unsigned int state) {
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