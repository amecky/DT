#pragma once
#include "ParticleArray.h"
#include <vector>

struct EmitterData {

	int count;

	float ttl;
};

typedef void(*ParticleGenerator)(const ParticleDataBuffer&, int, ParticleArray*, int, int);

//void createDefaultParticles(const ParticleDataBuffer& data,int index,ParticleArray* array, int start, int end);

struct RingData {

	float radius;

};

void createParticleRing(const ParticleDataBuffer& data, int index, ParticleArray* array, int start, int end);

struct RadialVelocityData {
	float velocity;
	float random;
};

void setRadialVelocity(const ParticleDataBuffer& data, int index, ParticleArray* array, int start, int end);

class ParticleEmitter {

typedef std::vector<ParticleGenerator> Generators;
typedef std::vector<int> EmitterDataIndices;
public:
	ParticleEmitter() {
		_emitterData.allocate(1024);
	}
	~ParticleEmitter() {}
	void emitt(const v2& position,ParticleArray* array);
	void add(ParticleGenerator* generator) {
		_generators.push_back(*generator);
	}
	void createRing(float radius);
	void createRadialVelocity(float velocity,float random);
private:
	Generators _generators;
	ParticleDataBuffer _emitterData;
	EmitterDataIndices _emitterDataIndices;
};