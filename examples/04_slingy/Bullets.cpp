#include "Bullets.h"
#include "..\..\sprites\SpriteBatch.h"
#include <d3dx9.h>

BulletHandler::BulletHandler() {
	_bulletTexture = math::buildTexture(Rect(30, 200, 20, 10), 512.0f, 512.0f, true);
	_trailTexture = math::buildTexture(Rect(0, 160, 14, 4), 512.0f, 512.0f, true);
}

void BulletHandler::start(const v2& pos,float angle) {
	if (_bullets.countAlive + 1 < _bullets.count) {
		int idx = _bullets.countAlive;
		_bullets.wake(idx);
		_bullets.position[idx] = pos;
		_bullets.previous[idx] = pos;
		_bullets.velocity[idx] = math::get_radial_velocity(angle, 800.0f);
		_bullets.rotation[idx] = angle;
		_bullets.color[idx] = Color(255,192,0);
		_bullets.scale[idx] = v2(1,1);//v2(2.5f,1.5f);
	}
}

void BulletHandler::tick(float dt) {
	for (int i = 0; i < _bullets.countAlive; ++i) {
		_bullets.position[i] += _bullets.velocity[i] * dt;
		if ( _trails.countAlive + 1 < _trails.count ) {
			int idx = _trails.countAlive;
			_trails.wake(idx);
			_trails.position[idx] = _bullets.position[i];
			_trails.timer[idx] = 0.2f;
			_trails.rotation[idx] = _bullets.rotation[i];
			_trails.color[idx] = Color(255,90,0,255);
			_trails.scale[idx] = v2(1.0f,1.2f);
		}
		_bullets.previous[i] = _bullets.position[i];
	}

	int cnt = 0;
	while (cnt < _bullets.countAlive) {
		v2 p = _bullets.position[cnt];
		if (p.x < 30.0f || p.x > 990.0f || p.y < 30.0f || p.y > 730.0f) {
			_bullets.kill(cnt);
			//_particles->start(p);
		}
		++cnt;
	}

	for (int i = 0; i < _trails.countAlive; ++i) {
		float norm = _trails.timer[i] / 0.2f;
		float scale = 0.4f + norm * 0.6f;
		_trails.scale[i].y = scale;
		_trails.color[i].a = 0.2f + norm * 0.7f;
	}

	cnt = 0;
	while (cnt < _trails.countAlive) {
		_trails.timer[cnt] -= dt;
		if ( _trails.timer[cnt] <= 0.0f ) {
			_trails.kill(cnt);
		}
		++cnt;
	}
}

void BulletHandler::render() {
	for (int i = 0; i < _trails.countAlive; ++i) {
		sprites::draw(_trails.position[i], _trailTexture,_trails.rotation[i],_trails.scale[i].x,_trails.scale[i].y,_trails.color[i]);
	}
	for (int i = 0; i < _bullets.countAlive; ++i) {
		sprites::draw(_bullets.position[i], _bulletTexture,_bullets.rotation[i],_bullets.scale[i].x,_bullets.scale[i].y,_bullets.color[i]);
	}
}
