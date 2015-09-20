#include "Asteroid.h"
#include <Matrix.h>
#include "..\..\renderer\BatchRenderer.h"
#include "..\..\utils\Log.h"

const int NUM_POINTS[] = {11,9,7,5};
const int TEX_OFFSET_TOP[] = {0, 10, 20, 30};
const float RADIUS[] = {0.9f,0.6f,0.4f,0.2f};
const float RADIUS_VARIANCE[] = {0.1f,0.1f,0.05f,0.05f};
const float VELOCITIES[] = { 2.0f,2.0f,2.0f,2.0f };
const float DELTA_RADIUS[] = { 0.12f, 0.11f, 0.1f, 0.1f };
const Rect BOUNDING_BOX = Rect(-2.5f, -3.8f, 7.5f, 5.0f);

void Asteroid::set(const AsteroidSettings& settings) {
	_settings = settings;
}

void Asteroid::create(int type,const v3& pos) {
	int numPoints = NUM_POINTS[type];
	float radius = RADIUS[type];
	float deg = D3DX_PI * 2.0f / static_cast<float>(numPoints);
	_position = pos;
	for ( int i = 0; i < numPoints; ++i ) {
		_points[i].angle = deg * static_cast<float>(i);
		_points[i].radiusDiff = math::random(-RADIUS_VARIANCE[type], RADIUS_VARIANCE[type]);
		_points[i].timer = math::random(0.0f, D3DX_PI);
	}
	float angle = math::random(0.0f,D3DX_PI * 2.0f);
	_velocity = math::get_radial_velocity(angle, VELOCITIES[type], _settings.velocityVariance);
	_radius = radius;
	_numPoints = numPoints;
	_timer = 0.0f;
	_type = type;
	_mode = AM_FLYING;
}

bool Asteroid::hit() {
	/*
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
	*/
	return false;
}

void Asteroid::tick(float dt) {
	_timer += dt;
	
	if ( _mode == AM_FLYING ) {
		_position += _velocity * dt;
		bool bouncing = false;
		if ( _position.x < ( BOUNDING_BOX.left + _radius) ) {
			_velocity.x *= -1.0f;
			bouncing = true;
		}
		if ( _position.x > ( BOUNDING_BOX.right - _radius) ) {
			_velocity.x *= -1.0f;
			bouncing = true;
		}
		if ( _position.y < ( BOUNDING_BOX.top + _radius) ) {
			_velocity.y *= -1.0f;
			bouncing = true;
		}
		if ( _position.y > ( BOUNDING_BOX.bottom - _radius) ) {
			_velocity.y *= -1.0f;
			bouncing = true;
		}
		if (bouncing) {
			_position += _velocity * dt;
		}
		for ( int i = 0; i < _numPoints; ++i ) {
			AsteroidPoint& p = _points[i];
			p.angle += D3DX_PI * dt * _settings.angularSpeed;
			p.timer += dt;
			p.radiusDiff = sin(p.timer * _settings.radiusSpeed) * RADIUS_VARIANCE[_type];
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

	drawLine(v3(BOUNDING_BOX.left, BOUNDING_BOX.bottom, 0.0f), v3(BOUNDING_BOX.left, BOUNDING_BOX.top, 0.0f), 0.1f);
	drawLine(v3(BOUNDING_BOX.right, BOUNDING_BOX.bottom, 0.0f), v3(BOUNDING_BOX.right, BOUNDING_BOX.top, 0.0f), 0.1f);
	drawLine(v3(BOUNDING_BOX.left, BOUNDING_BOX.top, 0.0f), v3(BOUNDING_BOX.right, BOUNDING_BOX.top, 0.0f), 0.1f);
	drawLine(v3(BOUNDING_BOX.left, BOUNDING_BOX.bottom, 0.0f), v3(BOUNDING_BOX.right, BOUNDING_BOX.bottom, 0.0f), 0.1f);

	if ( _mode == AM_FLYING ) {
		for ( int i = 0; i < _numPoints - 1; ++i ) {
			float ra = _radius + _points[i].radiusDiff;
			float rb = _radius + _points[i+1].radiusDiff;
			drawSegment(_points[i].angle,ra, _points[i + 1].angle, rb);
		}
		float ra = _radius + _points[_numPoints - 1].radiusDiff;
		float rb = _radius + _points[0].radiusDiff;
		drawSegment(_points[_numPoints - 1].angle,ra, _points[0].angle, rb);
	}
	else {
		for ( int i = 0; i < _numLines; ++i ) {
			//sprites::draw(_lines[i].center,_lines[i].texture,_lines[i].rotation);
		}
	}
}

void Asteroid::buildLine(const v3& start,const v3& end,Line* line) {
	/*
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
	line->texture = math::buildTexture(Rect(TEX_OFFSET_TOP[_type],150,(int)line->length,5),512.0f,512.0f,true);	
	line->velocity = math::get_radial_velocity(line->rotation,50.0f);
	*/
}

void Asteroid::drawSegment(float alpha, float ra, float beta, float rb) {
	v3 p[4];
	float dr = DELTA_RADIUS[_type];
	p[0] = v3(_position.x + cos(beta) * rb, _position.y + sin(beta) * rb, 0.0f);
	p[1] = v3(_position.x + cos(beta) * (rb + dr), _position.y + sin(beta) * (rb + dr), 0.0f);
	p[2] = v3(_position.x + cos(alpha) * (ra + dr), _position.y + sin(alpha) * (ra + dr), 0.0f);
	p[3] = v3(_position.x + cos(alpha) * ra, _position.y + sin(alpha) * ra, 0.0f);
	Texture texture = math::buildTexture(Rect(TEX_OFFSET_TOP[_type], 0, 40, 7), 512.0f, 512.0f, false);
	batch::drawQuad(p, texture);
}

void Asteroid::drawLine(const v3& start,const v3& end,float thickness) {
	v3 p[4];
	Vector3f d = end - start;
	Vector3f dn = rotationZ<float>(D3DX_PI * 0.5f) * d;
	dn = normalize(dn);
	dn *= thickness;
	Vector3f sn = start + dn;
	Vector3f en = end + dn;
	p[0] = v3(start.x, start.y, start.z);
	p[1] = v3(sn.x, sn.y, sn.z);
	p[2] = v3(en.x, en.y, en.z);
	p[3] = v3(end.x, end.y, end.z);
	Texture texture = math::buildTexture(Rect(TEX_OFFSET_TOP[_type], 0, 40, 7), 512.0f, 512.0f, false);
	batch::drawQuad(p, texture);
}