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
	float random;

 	RingData() : radius(10.0f) , random(0.0f) {}

	RingData(float _radius,float _random) : radius(_radius) , random(_random) {}

};

void createParticleRing(const ParticleDataBuffer& data, int index, ParticleArray* array, int start, int end);

struct RadialVelocityData {

	float velocity;
	float random;

	RadialVelocityData() : velocity(0.0f) , random(0.0f) {}
	RadialVelocityData(float _v,float _r) : velocity(_v) , random(_r) {}

};

void setRadialVelocity(const ParticleDataBuffer& data, int index, ParticleArray* array, int start, int end);

struct TTLData {

	float ttl;
	float random;

	TTLData() : ttl(1.0f) , random(0.0f) {}

	explicit TTLData(float _ttl) : ttl(_ttl) , random(0.0f) {}

	TTLData(float _ttl,float _random) : ttl(_ttl) , random(_random) {}
};

void setLifecycle(const ParticleDataBuffer& data, int index, ParticleArray* array, int start, int end);


struct GeneratorDefinition {

	char* name;
	ParticleGenerator function;

};

struct Generator {

	int dataIndex;
	int definitionIndex;

};

class ParticleEmitter {

typedef std::vector<Generator> Generators;

public:
	ParticleEmitter() {
		_emitterData.allocate(1024);
	}
	~ParticleEmitter() {}
	void emitt(const v2& position,ParticleArray* array);
	void addGenerator(const char* name) {
		Generator g;
		g.dataIndex = -1;
		g.definitionIndex = findFunctionIndex(name);
		_generators.push_back(g);
	}
	template<class T>
	void addGenerator(const char* name,const T& data) {
		Generator g;
		g.dataIndex = _emitterData.add(data);
		g.definitionIndex = findFunctionIndex(name);
		_generators.push_back(g);
	}
private:
	int findFunctionIndex(const char* name);
	Generators _generators;
	ParticleDataBuffer _emitterData;
};