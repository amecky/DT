#include "StdAfx.h"
#include "TestApp.h"

BaseApp *app = new TestApp(); 

TestApp::TestApp(void) {
}


TestApp::~TestApp(void) {

}


void TestApp::loadContent() {
	BlendState bs;
	bs.blendEnable = true;
	bs.srcFactorRGB = BL_ONE;
	bs.dstFactorRGB = BL_ONE;
	bs.blendOp = BM_ADD;
	bs.srcFactorAlpha = BL_ONE;
	bs.dstFactorAlpha = BL_ONE_MINUS_SRC_ALPHA;
	bs.alphaRef = (DWORD)0x0000000f;
	bs.alphaFunc = ALPHA_GEQ;
	
	_addBlendState = _dx.createBlendState(bs);
	int tid = _dx.loadTexture("blurred");
	_particles = new ParticleSystem(&_dx,1024,tid);
	emitter.add(new RingGenerator(2.0f));
	emitter.add(new RadialVelocityGenerator(1.0f));
	_particles->setEmitter(&emitter);
	_particles->setTTL(3.0f,0.0f);
	_particles->activateMovement();	
	_particles->colorize(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),D3DXCOLOR(0.2f,0.0f,0.0f,0.2f));
	_particles->scale(1.0f,0.1f);
	_particles->start(Vector3f(0,0,0));
}

void TestApp::tick(float dt) {
	_particles->tick(dt);
}

void TestApp::render() {
	_dx.setBlendState(_addBlendState);
	_particles->render();
}

void TestApp::onChar(char ascii,unsigned int state) {
	if ( ascii == '1' ) {
		_particles->start(Vector3f(0,0,0));
	}
}