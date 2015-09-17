#include "..\..\StdAfx.h"
#include "CubeTest.h"
#include "..\..\renderer\render_types.h"
#include "..\..\World.h"
#include "..\..\utils\Log.h"
#include "..\..\renderer\gfx.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\renderer\Shader.h"
#include "..\..\sprites\SpriteBatch.h"
#include "..\..\math\mathutils.h"
//BaseApp *app = new CubeTest();

CubeTest::CubeTest() {
}


CubeTest::~CubeTest() {
}


void CubeTest::loadContent() {
	sprites::intialize("ref_256");
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
	_timer += dt;
}

void CubeTest::render() {
	sprites::begin();
	float angle = 0.0f;
	if ( _rotating ) {
		angle = _timer;
	}
	float s = 1.0f;
	if ( _scaling ) {
		s = 1.0f + sin(_timer) * 0.2f;		
	}
	sprites::draw(v2(400,300),math::buildTexture(Rect(0,0,256,256),256.0f,256.0f,true),angle,s,s);	
	sprites::end();
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
	}
}