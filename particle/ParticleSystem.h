#pragma once
#include "..\renderer\DX.h"
#include "ParticleArray.h"
#include "..\renderer\Mesh.h"
#include "ParticleModifier.h"
#include "ParticleEmitter.h"
#include <vector>

struct ParticleSystemInstance {

	ParticleArray array;
	ParticleEmitter* emitter;

};

class ParticleSystem {

typedef std::vector<ParticleModifier> Modifiers;
typedef std::vector<int> ModifierDataIndices;

public:
	ParticleSystem(DX* dx,int max,int texture_id);
	~ParticleSystem() {}
	void tick(float dt);
	void render();
	void setTTL(float ttl,float random);
	void activateMovement();
	void colorize(D3DXCOLOR start,D3DXCOLOR end);
	void scale(float min,float max);
	void add(ParticleModifier* m) {
		_modifiers.push_back(*m);
	}
	void start(const Vector3f& start);
	void setEmitter(ParticleEmitter* emitter) {
		_emitter = emitter;
	}
private:
	DX* _dx;
	int _max;
	int _buffer_id;
	int _texture_id;
	ParticleArray _array;
	PCTMeshData* _data;
	Modifiers _modifiers;
	ParticleEmitter* _emitter;
	ModifierData _modifierData;
	ModifierDataIndices _modifierDataIndices;
};