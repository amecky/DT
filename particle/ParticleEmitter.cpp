#include "ParticleEmitter.h"

void createDefaultParticles(const v2& position,ParticleArray* array, int start, int end) {
	for ( int i = start; i < end; ++i ) {
		array->color[i] = Color(1.0f,1.0f,1.0f,1.0f);
		array->scale[i] = Vector2f(1,1);
		array->rotation[i] = 0.0f;
		array->timer[i] = Vector3f(0,1,2);
		array->random[i] = 1.0f;
		array->acceleration[i] = v2(0,0);
		array->velocity[i] = v2(0, 0);
		array->position[i] = position;
		array->type[i] = 1;
	}
}

void createParticleRing(const ParticleDataBuffer& data, int index, ParticleArray* array, int start, int end) {
	RingData myData;
	if (data.get(index, &myData, sizeof(RingData))) {
		int d = end - start;
		float step = 2.0f * D3DX_PI / static_cast<float>(d);
		float angle = 0.0f;
		for (int i = start; i < end; ++i) {
			float x = myData.radius * cos(angle);
			float y = myData.radius * sin(angle);
			v2 p(x, y);
			array->normal[i] = normalize(p);
			array->position[i] += p;
			angle += step;
		}
	}
}

void setRadialVelocity(const ParticleDataBuffer& data, int index, ParticleArray* array, int start, int end) {
	RadialVelocityData myData;
	if (data.get(index, &myData, sizeof(RadialVelocityData))) {
		for (int i = start; i < end; ++i) {
			array->velocity[i] = array->normal[i] * myData.velocity;
		}
	}
}

void ParticleEmitter::createRing(float radius) {
	RingData data;
	data.radius = radius;
	int id = _emitterData.add(data);
	_emitterDataIndices.push_back(id);
	_generators.push_back(&createParticleRing);
}

void ParticleEmitter::createRadialVelocity(float velocity,float random) {
	RadialVelocityData data;
	data.velocity = velocity;
	data.random = random;
	int id = _emitterData.add(data);
	_emitterDataIndices.push_back(id);
	_generators.push_back(&setRadialVelocity);
}

void ParticleEmitter::emitt(const v2& position, ParticleArray* array) {
	int s = array->countAlive;
	int e = s + 16;
	if ( e - s > 0 ) {
		if ( e >= array->count ) {
			e = array->count;
		}
		createDefaultParticles(position,array, s, e);
		for (size_t i = 0; i < _generators.size(); ++i) {
			(*_generators[i])(_emitterData, _emitterDataIndices[i], array, s, e);
		}
		for (int i = s; i < e; ++i) {
			array->wake(i);
		}	
	}
}