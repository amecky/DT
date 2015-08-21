#pragma once
#include "ParticleArray.h"
#include <vector>

struct EmitterData {

	int count;

	float ttl;
};

class ParticleGenerator {

public:
	ParticleGenerator() {}
	virtual ~ParticleGenerator() {}
	virtual void create(ParticleArray* array,int start,int end) = 0;
};

class DefaultParticleGenerator : public ParticleGenerator {

public:
	DefaultParticleGenerator() {}
	virtual ~DefaultParticleGenerator() {}
	void create(ParticleArray* array,int start,int end);

};

class RingGenerator : public ParticleGenerator {

public:
	RingGenerator(float radius) : ParticleGenerator() , _radius(radius) {}
	virtual ~RingGenerator() {}
	void create(ParticleArray* array,int start,int end);
private:
	float _radius;
};

class RadialVelocityGenerator : public ParticleGenerator {

public:
	RadialVelocityGenerator(float v) : ParticleGenerator() , _velocity(v) {}
	virtual ~RadialVelocityGenerator() {}
	void create(ParticleArray* array,int start,int end);
private:
	float _velocity;
};

class ParticleEmitter {

typedef std::vector<ParticleGenerator*> Generators;

public:
	ParticleEmitter() {
		_generators.push_back(new DefaultParticleGenerator());
	}
	~ParticleEmitter() {}
	void emitt(ParticleArray* array);
	void add(ParticleGenerator* generator) {
		_generators.push_back(generator);
	}
private:
	Generators _generators;
};