#pragma once
#include "..\..\sprites\SpriteBatch.h"

enum AsteroidMode {
	AM_FLYING,
	AM_EXPLODING
};

struct Line {
	v3 start;
	v3 end;
	v3 center;
	float rotation;
	float rotationVelocity;
	float length;
	v3 velocity;
	Texture texture;
};

struct AsteroidSettings {
	float angularSpeed;
	float radiusSpeed;
	float velocity;
	float velocityVariance;
};

class Asteroid {

struct AsteroidPoint {
	float angle;
	float radiusDiff;
	float timer;
};

public:
	Asteroid() {}
	~Asteroid() {}
	void set(const AsteroidSettings& settings);
	void create(int type,const v3& pos);
	void tick(float dt);
	void render();
	bool hit();
	const v3& getPosition() const {
		return _position;
	}
	const v3& getVelocity() const {
		return _velocity;
	}
	const float getRadius() const {
		// FIXME: calculate max radius
		return _radius;
	}
	bool isDead() const;
private:
	void drawLine(const v3& start,const v3& end,float thickness);
	void drawSegment(float alpha, float ra, float beta, float rb);
	void buildLine(const v3& start,const v3& end,Line* line);
	float _radius;
	v3 _position;
	v3 _velocity;
	AsteroidPoint _points[12];	
	int _numPoints;
	Line _lines[9];
	int _numLines;
	float _timer;
	int _type;
	AsteroidSettings _settings;
	AsteroidMode _mode;
};