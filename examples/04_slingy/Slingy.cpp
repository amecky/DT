#include "..\..\StdAfx.h"
#include "Slingy.h"
#include "..\..\renderer\render_types.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\sprites\SpriteBatch.h"
#include "..\..\utils\Log.h"
#include "..\..\math\mathutils.h"

BaseApp *app = new Slingy();

Slingy::Slingy() {
	_settings.tickCamera = false;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	_settings.clearColor = Color(0,0,0);
	//_CrtSetBreakAlloc(339);
	_firing = false;
	_fireTimer = 0.0f;
}


Slingy::~Slingy() {
	delete _particles;
	sprites::shutdown();
}


void Slingy::loadContent() {
	sprites::intialize("array");
	_headTexture = math::buildTexture(Rect(30,0,50,50),512.0f,512.0f,true);
	_tailTexture = math::buildTexture(Rect(0,0,22,22),512.0f,512.0f,true);
	_bulletTexture = math::buildTexture(Rect(0, 150, 6, 6), 512.0f, 512.0f, true);
	_startPos = v2(12*30+45,9*30+45);

	_head.position = _startPos;
	_head.aabBox = AABBox(_head.position,v2(15,15));
	_head.angle = 0.0f;
	v2 p = _head.position;
	for ( int i = 0; i < 10; ++i ) {
		Tail t;
		p.x -= 22.0f;
		t.position = p;
		t.aabBox = AABBox(t.position,v2(10,10));
		t.angle = 0.0f;
		_tails.push_back(t);
	}

	_particles = new ParticleSystem(256, 0);
	_particles->setTexture(math::buildTexture(0, 100, 20, 20, 512.0f, 512.0f, true));
	// generators
	_particles->addGenerator("ring",RingData(2.0f,0.0f));
	_particles->addGenerator("radial_velocity",RadialVelocityData(180.0f,120.0f));
	_particles->addGenerator("ttl",TTLData(0.6f,0.2f));
	// modifiers
	_particles->addModifier("lifecycle");
	_particles->addModifier("move");
	_particles->addModifier("scale",ParticleScaleData(0.4f,0.2f));
	_particles->addModifier("colorize",ParticleColorData(Color(255,255,0),Color(255,0,0,64)));
	/*
	ParticleChannel channels[] = {
		{"pos",0},
		{"velocity",-1},
		{"scale",2},
		{"rotation",1},
		{"color",3},
		{"acceleration",-1},
	};
	_particles->initialize(channels,6);
	*/
}

void Slingy::moveTail(float dt) {
	v2 parent = _head.position;
	for ( size_t i = 0; i < _tails.size(); ++i ) {
		v2 diff = parent - _tails[i].position;
		if ( sqr_length(diff) > 22.0f ) {
			float tooFar = length(diff) - 22.0f;
			v2 translate = normalize(diff) * tooFar;
			_tails[i].position += translate;
			_tails[i].aabBox.translate(_tails[i].position);
			_tails[i].angle = math::get_target_angle(parent,_tails[i].position);
		}
		parent = _tails[i].position;
	}
}

void Slingy::tick(float dt) {

	_timer += dt;

	_particles->tick(dt);

	v2 v;
	if (GetAsyncKeyState('W') & 0x8000) {
		v += Vector2f(0, 1);
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		v += Vector2f(0, -1);
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		v += Vector2f(-1, 0);
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		v += Vector2f(1, 0);
	}
	

	v2 mp = gfx::getMousePos();
	v2 diff = mp - _head.position;
	_head.angle = math::calculateRotation(diff);
	_head.position += v * dt * 150.0f;

	//moveTail(dt);

	_grid.tick(dt);
	_grid.setHighlight(_head.position);

	if (_firing) {
		_fireTimer += dt;
		if (_fireTimer > 0.4f) {
			LOG << "fire!!!!!";
			_fireTimer -= 0.4f;
			// fire bullet
			if (_bullets.countAlive + 1 < _bullets.count) {
				int idx = _bullets.countAlive;
				_bullets.wake(idx);
				_bullets.position[idx] = _head.position;
				_bullets.velocity[idx] = math::get_radial_velocity(_head.angle, 800.0f);
			}
		}
	}

	for (int i = 0; i < _bullets.countAlive; ++i) {
		_bullets.position[i] += _bullets.velocity[i] * dt;
	}

	int cnt = 0;
	while (cnt < _bullets.countAlive) {
		v2 p = _bullets.position[cnt];
		if (p.x < 0.0f || p.x > 1020.0f || p.y < 0.0f || p.y > 768.0f) {
			_bullets.kill(cnt);
		}
		++cnt;
	}
}

void Slingy::render() {
	sprites::begin();

	_grid.render();
	sprites::draw(_head.position, _headTexture, _head.angle, 1.0f, 1.0f, Color(192,192,192));
	for (int i = 0; i < _bullets.countAlive; ++i) {
		sprites::draw(_bullets.position[i], _bulletTexture);
	}
	//for ( size_t i = 0; i < _tails.size(); ++i ) {
		//sprites::draw(_tails[i].position, _tailTexture, _tails[i].angle, 1.0f, 1.0f, Color(192, 0, 192));
	//}
	char buffer[32];
	v2 mp = gfx::getMousePos();
	sprintf(buffer, "%3.2f %3.2f", mp.x, mp.y);
	sprites::drawText(buffer, 100, 740,Color(192,0,0));
	_particles->render();
	sprites::end();
}

void Slingy::onChar(char ascii, unsigned int state) {
	if (ascii == '2'){
		_particles->start(v2(400, 300));
	}
}

void Slingy::onButton(int button, ButtonState state) {
	if (button == 0) {
		if (state == BS_DOWN) {
			_firing = true;
		}
		else if (state == BS_UP) {
			_firing = false;
		}
	}
}