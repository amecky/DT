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
	D3DXCOLOR start;
	D3DXCOLOR end;
};

void colorizeParticle(const ModifierData& data, int dataIndex, ParticleArray* array, float dt);

struct ParticleScaleData {
	float minSize;
	float maxSize;
};

void scaleParticle(const ModifierData& data, int dataIndex, ParticleArray* array, float dt);
/*

// -------------------------------------------------------
// ParticlePositionModifier
// -------------------------------------------------------
class ParticleScaleModifier : public ParticleModifier {

public:
	ParticleScaleModifier(const Vector2f& start,const Vector2f& end) : ParticleModifier() , _start(start) , _end(end) {}
	virtual ~ParticleScaleModifier() {}

	void update(ParticleArray* array,float dt) {
		for ( int i = 0; i < array->countAlive; ++i ) {
			float n = array->timer[i].y;
			array->scale[i] = _start * (1.0f - n) + _end * n;
		}
	}
private:
	Vector2f _start;
	Vector2f _end;
};

// -------------------------------------------------------
// ParticlePositionModifier
// -------------------------------------------------------
class ParticleColorModifier : public ParticleModifier {

public:
	ParticleColorModifier(D3DXCOLOR start,D3DXCOLOR end) : ParticleModifier() , _start(start) , _end(end) {}
	virtual ~ParticleColorModifier() {}

	void update(ParticleArray* array,float dt) {
		for ( int i = 0; i < array->countAlive; ++i ) {
			float n = array->timer[i].y;
			array->color[i].r = _start.r * (1.0f - n) + _end.r * n;
			array->color[i].g = _start.g * (1.0f - n) + _end.g * n;
			array->color[i].b = _start.b * (1.0f - n) + _end.b * n;
			array->color[i].a = _start.a * (1.0f - n) + _end.a * n;
		}
	}
private:
	D3DXCOLOR _start;
	D3DXCOLOR _end;
};
*/