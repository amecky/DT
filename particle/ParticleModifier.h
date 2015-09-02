#pragma once
#include "ParticleArray.h"

struct ModifierData {

	char* buffer;
	int index;
	int num;
	int entries[64];

	ModifierData() : buffer(0) , index(0) , num(0) {}

	~ModifierData() {
		if (buffer != 0) {
			delete[] buffer;
		}
	}

	void allocate(int size) {
		buffer = new char[size];
	}

	template<class T>
	const bool get(int id,T* t,int size) const {
		int idx = entries[id];
		char* data = buffer + idx;
		memcpy(t, data, size);
		return true;
	}

	template<class T>
	int add(const T& t) {
		int current = index;
		entries[num++] = current;
		char* data = buffer + index;
		memcpy(data, &t, sizeof(t));
		index += sizeof(t);
		return num - 1;
	}
};

const enum ModifierDataIndex {
};

typedef void(*ParticleModifier)(const ModifierData&,int,ParticleArray*, float);

struct LiveOverTimeData {

	float ttl;

	LiveOverTimeData() : ttl(1.0f) {}

	explicit LiveOverTimeData(float _ttl) : ttl(_ttl) {}
};

void lifeOverTime(const ModifierData& data, int dataIndex, ParticleArray* array, float dt);
void moveParticle(const ModifierData& data, int dataIndex, ParticleArray* array, float dt);

struct ParticleColorData {
	Color start;
	Color end;
};

void colorizeParticle(const ModifierData& data, int dataIndex, ParticleArray* array, float dt);

struct ParticleScaleData {
	float minSize;
	float maxSize;
};

void scaleParticle(const ModifierData& data, int dataIndex, ParticleArray* array, float dt);
