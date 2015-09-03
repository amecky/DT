#include "ParticleEmitter.h"

const int NUM_DEFINITIONS = 3;

const GeneratorDefinition DEFINITIONS[] = {
	{"ring",&createParticleRing},
	{"radial_velocity",&setRadialVelocity},
	{"ttl",&setLifecycle},
	//{"colorize",&colorizeParticle},
};

void createDefaultParticles(const v2& position,ParticleArray* array, int start, int end) {
	for ( int i = start; i < end; ++i ) {
		array->color[i] = Color(1.0f,1.0f,1.0f,1.0f);
		array->scale[i] = v2(1,1);
		array->rotation[i] = 0.0f;
		array->timer[i] = v3(0,1,2);
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
			float r = myData.radius + math::random(-myData.random,myData.random);
			float x = r * cos(angle);
			float y = r * sin(angle);
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
			float v = myData.velocity + math::random(-myData.random,myData.random);
			array->velocity[i] = array->normal[i] * v;
		}
	}
}

void setLifecycle(const ParticleDataBuffer& data, int index, ParticleArray* array, int start, int end) {
	TTLData myData;
	if (data.get(index, &myData, sizeof(TTLData))) {
		for (int i = start; i < end; ++i) {
			array->timer[i].z = myData.ttl + math::random(-myData.random,myData.random);
		}
	}
}


int ParticleEmitter::findFunctionIndex(const char* name) {
	for ( int i =0; i < NUM_DEFINITIONS; ++i ) {
		if ( strcmp(name,DEFINITIONS[i].name) == 0 ) {
			return i;
		}
	}
	return -1;
}

void ParticleEmitter::emitt(const v2& position, ParticleArray* array) {
	int s = array->countAlive;
	int e = s + 64;
	if ( e - s > 0 ) {
		if ( e >= array->count ) {
			e = array->count;
		}
		createDefaultParticles(position,array, s, e);
		for (size_t i = 0; i < _generators.size(); ++i) {
			const Generator& g = _generators[i];
			(*DEFINITIONS[g.definitionIndex].function)(_emitterData, g.dataIndex, array, s, e);
		}
		for (int i = s; i < e; ++i) {
			array->wake(i);
		}	
	}
}