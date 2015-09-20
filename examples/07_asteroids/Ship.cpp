#include "Ship.h"
#include "..\..\renderer\BatchRenderer.h"

Ship::Ship() {
	_position = v3(0, 0, 0);
	_angle = 0.0f;
}


Ship::~Ship() {
}

void Ship::rotate(float angle) {
	_angle += angle;
}

void Ship::render() {
	float radius = 0.2f;
	drawSegment(DEGTORAD(90.0f) + _angle, radius, DEGTORAD(225.0f) + _angle, radius);
	drawSegment(DEGTORAD(225.0f) + _angle, radius, DEGTORAD(315.0f) + _angle, radius);
	drawSegment(DEGTORAD(315.0f) + _angle, radius, DEGTORAD(90.0f) + _angle, radius);
}

void Ship::move(const v3& v) {
	_position += v;
}

void Ship::drawSegment(float alpha, float ra, float beta, float rb) {
	v3 p[4];
	float dr = 0.15f;
	p[0] = v3(_position.x + cos(beta) * rb, _position.y + sin(beta) * rb, 0.0f);
	p[1] = v3(_position.x + cos(beta) * (rb + dr), _position.y + sin(beta) * (rb + dr), 0.0f);
	p[2] = v3(_position.x + cos(alpha) * (ra + dr), _position.y + sin(alpha) * (ra + dr), 0.0f);
	p[3] = v3(_position.x + cos(alpha) * ra, _position.y + sin(alpha) * ra, 0.0f);
	Texture texture = math::buildTexture(Rect(40, 0, 40, 7), 512.0f, 512.0f, false);
	batch::drawQuad(p, texture);
}