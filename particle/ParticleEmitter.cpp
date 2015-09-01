#include "ParticleEmitter.h"

void DefaultParticleGenerator::create(ParticleArray* array,int start,int end) {
	for ( int i = start; i < end; ++i ) {
		array->color[i] = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		array->scale[i] = Vector2f(1,1);
		array->rotation[i] = 0.0f;
		array->timer[i] = Vector3f(0,1,2);
		array->random[i] = 1.0f;
		array->acceleration[i] = v2(0,0);
		array->velocity[i] = v2(0, 0);
		array->position[i] = v2(0, 0);
		array->type[i] = 1;
	}
}

void RingGenerator::create(ParticleArray* array,int start,int end) {
	int d = end - start;
	float step = 2.0f * D3DX_PI / static_cast<float>(d);
	float angle = 0.0f;
	for ( int i = start; i < end; ++i ) {
		float x = _radius * cos(angle);
		float y = _radius * sin(angle);
		array->position[i] = v2(x, y);
		angle += step;
	}
}

void RadialVelocityGenerator::create(ParticleArray* array,int start,int end) {
	for ( int i = start; i < end; ++i ) {
		v2 p = array->position[i];
		array->velocity[i] = normalize(p) * _velocity;
	}
}

void ParticleEmitter::emitt(ParticleArray* array) {
	int s = array->countAlive;
	int e = s + 16;
	if ( e - s > 0 ) {
		if ( e >= array->count ) {
			e = array->count;
		}
		for (size_t i = 0; i < _generators.size(); ++i) {
			_generators[i]->create(array,s, e);
		}
		for (int i = s; i < e; ++i) {
			array->wake(i);
		}	
	}
}