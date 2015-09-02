#include "ParticleModifier.h"

void lifeOverTime(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt) {
	LiveOverTimeData myData;
	if (data.get(dataIndex, &myData, sizeof(LiveOverTimeData))) {
		for (int i = 0; i < array->countAlive; ++i) {
			array->timer[i].x += dt;
			array->timer[i].y = array->timer[i].x / array->timer[i].z;
		}
		int cnt = 0;
		while (cnt < array->countAlive) {
			if (array->timer[cnt].x > array->timer[cnt].z) {
				array->kill(cnt);
			}
			++cnt;
		}
	}
}

void moveParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt) {
	for ( int i = 0; i < array->countAlive; ++i ) {
		array->velocity[i] += array->acceleration[i] * dt;
		array->position[i] += array->velocity[i] * dt;
	}
}

void colorizeParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt) {
	ParticleColorData myData;
	if (data.get(dataIndex, &myData, sizeof(ParticleColorData))) {
		for ( int i = 0; i < array->countAlive; ++i ) {
			float n = array->timer[i].y;
			array->color[i].r = myData.start.r * (1.0f - n) + myData.end.r * n;
			array->color[i].g = myData.start.g * (1.0f - n) + myData.end.g * n;
			array->color[i].b = myData.start.b * (1.0f - n) + myData.end.b * n;
			array->color[i].a = myData.start.a * (1.0f - n) + myData.end.a * n;
		}
	}
}

void scaleParticle(const ParticleDataBuffer& data, int dataIndex, ParticleArray* array, float dt) {
	ParticleScaleData myData;
	if (data.get(dataIndex, &myData, sizeof(ParticleScaleData))) {
		for ( int i = 0; i < array->countAlive; ++i ) {
			float n = array->timer[i].y;
			float s = myData.minSize * (1.0f - n) + myData.maxSize * n;
			array->scale[i].x = s;
			array->scale[i].y = s;
		}
	}
}