#include "Asteroid.h"

const int NUM_POINTS[] = {9,7,5,3};
const int TEX_OFFSET_TOP[] = {0, 30, 40, 50};
const int RADIUS[] = {70, 55, 40, 25};
const int RADIUS_VARIANCE[] = {10, 10, 5, 5};

void Asteroid::set(const AsteroidSettings& settings) {
	_settings = settings;
}

void Asteroid::create(int type,const v2& pos) {
	int numPoints = NUM_POINTS[type];
	int radius = RADIUS[type];
	float deg = D3DX_PI * 2.0f / static_cast<float>(numPoints);
	_position = pos;
	for ( int i = 0; i < numPoints; ++i ) {
		float r = math::random(radius - RADIUS_VARIANCE[type], radius + RADIUS_VARIANCE[type]);
		float x = pos.x + cos(deg * i) * r;
		float y = pos.x + sin(deg * i) * r;
		_points[i].angle = deg * static_cast<float>(i);
		_points[i].radiusDiff = radius - r;
		_points[i].position = v2(x,y);
	}
	float angle = math::random(0.0f,D3DX_PI * 20.0f);
	float vr = math::random(_settings.velocity - _settings.velocityVariance,_settings.velocity+_settings.velocityVariance);
	_velocity = math::get_radial_velocity(angle,50.0f);
	_radius = radius;
	_numPoints = numPoints;
	_timer = 0.0f;
	_type = type;
	_mode = AM_FLYING;
}

bool Asteroid::hit() {
	if ( _mode != AM_EXPLODING ) {
		_mode = AM_EXPLODING;
		_timer = 0.0f;
		// split lines
		int cnt = 0;
		for ( int i = 1; i < _numPoints; ++i ) {
			Line* line = &_lines[cnt++];
			buildLine(_points[i-1].position,_points[i].position,line);
		}
		Line* line = &_lines[cnt++];
		buildLine(_points[_numPoints-1].position,_points[0].position,line);
		_numLines = cnt;
		return true;
	}
	return false;
}

void Asteroid::tick(float dt) {
	_timer += dt;
	if ( _mode == AM_FLYING ) {
		_position += _velocity * dt;
		if ( _position.x < ( 20.0f + _radius) ) {
			_velocity.x *= -1.0f;
		}
		if ( _position.x > ( 1000.0f - _radius) ) {
			_velocity.x *= -1.0f;
		}
		if ( _position.y < ( 20.0f + _radius) ) {
			_velocity.y *= -1.0f;
		}
		if ( _position.y > ( 720.0f - _radius) ) {
			_velocity.y *= -1.0f;
		}
		for ( int i = 0; i < _numPoints; ++i ) {
			AsteroidPoint& p = _points[i];
			p.angle += D3DX_PI * dt * _settings.angularSpeed;
			float r = _radius + p.radiusDiff * sin(_timer * _settings.radiusSpeed);
			float x = _position.x + cos(p.angle) * r;
			float y = _position.y + sin(p.angle) * r;
			p.position = v2(x,y);
		}
	}
	else {
		for ( int i = 0; i < _numLines; ++i ) {
			_lines[i].rotation += D3DX_PI * dt * _lines[i].rotationVelocity;
			_lines[i].center += _lines[i].velocity * dt;
		}

	}
}

bool Asteroid::isDead() const {
	if ( _mode != AM_EXPLODING ) {
		return false;
	}
	if ( _timer > 4.0f ) {
		return true;
	}
	return false;
}

void Asteroid::render() {
	if ( _mode == AM_FLYING ) {
		v2 center = _position;
		for ( int i = 1; i < _numPoints; ++i ) {
			drawLine(_points[i-1].position,_points[i].position);
			sprites::draw(_points[i].position,math::buildTexture(Rect(10,20,10,10),512.0f,512.0f,true));			
		}
		sprites::draw(_points[0].position,math::buildTexture(Rect(10,20,10,10),512.0f,512.0f,true));			
		drawLine(_points[_numPoints-1].position,_points[0].position);
	}
	else {
		for ( int i = 0; i < _numLines; ++i ) {
			sprites::draw(_lines[i].center,_lines[i].texture,_lines[i].rotation);
		}
	}
}

void Asteroid::buildLine(const v2& start,const v2& end,Line* line) {
	line->start = start;
	line->end = end;
	v2 diff = end - start;
	line->rotation = math::get_angle(v2(1,0),diff);
	line->center = start + diff * 0.5f;
	line->length = length(diff);
	line->rotationVelocity = math::random(0.2f,0.6f);
	if ( math::chanceRoll(50)) {
		line->rotationVelocity *= -1.0f;
	}
	line->texture = math::buildTexture(Rect(TEX_OFFSET_TOP[_type],0,(int)line->length,5),512.0f,512.0f,true);	
	line->velocity = math::get_radial_velocity(line->rotation,50.0f);
}

void Asteroid::drawLine(const v2& start,const v2& end) {
	v2 diff = end - start;
	float angle = math::get_angle(v2(1,0),diff);
	v2 center = start + diff * 0.5f;
	int l = length(diff);
	sprites::draw(center,math::buildTexture(Rect(TEX_OFFSET_TOP[_type],0,l,5),512.0f,512.0f,true),angle);
}