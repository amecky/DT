#pragma once
#include <Vector.h>

class Ship {

public:
	Ship();
	~Ship();
	void render();
	void rotate(float angle);
	void move(const v3& v);
private:
	void drawSegment(float alpha, float ra, float beta, float rb);
	v3 _position;
	float _angle;
};

