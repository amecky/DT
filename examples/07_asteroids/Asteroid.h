#pragma once
#include "..\..\sprites\SpriteBatch.h"

enum AsteroidMode {
	AM_FLYING,
	AM_EXPLODING
};

struct Line {
	v2 start;
	v2 end;
	v2 center;
	float rotation;
	float rotationVelocity;
	float length;
	v2 velocity;
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
	v2 position;
};

public:
	Asteroid() {}
	~Asteroid() {}
	void set(const AsteroidSettings& settings);
	void create(int type,const v2& pos);
	void tick(float dt);
	void render();
	bool hit();
	const v2& getPosition() const {
		return _position;
	}
	const v2& getVelocity() const {
		return _velocity;
	}
	const float getRadius() const {
		// FIXME: calculate max radius
		return _radius;
	}
	bool isDead() const;
private:
	void drawLine(const v2& start,const v2& end);
	void buildLine(const v2& start,const v2& end,Line* line);
	float _radius;
	v2 _position;
	v2 _velocity;
	AsteroidPoint _points[9];	
	int _numPoints;
	Line _lines[9];
	int _numLines;
	float _timer;
	int _type;
	AsteroidSettings _settings;
	AsteroidMode _mode;
};