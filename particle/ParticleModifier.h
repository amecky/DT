#pragma once
#include "ParticleArray.h"

typedef void(*ParticleModifier)(const ParticleDataBuffer&, int, ParticleArray*, float);



void lifeOverTime(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt);
void moveParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt);

struct ParticleColorData {

	Color start;
	Color end;

	ParticleColorData() : start(255,255,255) , end(255,255,255) {}

	ParticleColorData(const Color& s,const Color& e) : start(s) , end(e) {}

};

void colorizeParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt);

struct ParticleScaleData {

	float minSize;
	float maxSize;

	ParticleScaleData() : minSize(1.0f) , maxSize(1.0f) {}

	ParticleScaleData(float _min,float _max) : minSize(_min) , maxSize(_max) {}

};

void scaleParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt);
