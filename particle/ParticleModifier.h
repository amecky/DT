#pragma once
#include "ParticleArray.h"

typedef void(*ParticleModifier)(const ParticleDataBuffer&, int, ParticleArray*, float);

struct LiveOverTimeData {

	float ttl;

	LiveOverTimeData() : ttl(1.0f) {}

	explicit LiveOverTimeData(float _ttl) : ttl(_ttl) {}
};

void lifeOverTime(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt);
void moveParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt);

struct ParticleColorData {
	Color start;
	Color end;
};

void colorizeParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt);

struct ParticleScaleData {
	float minSize;
	float maxSize;
};

void scaleParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt);
