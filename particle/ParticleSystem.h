#pragma once
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
	ParticleSystem(int max,int texture_id);
	~ParticleSystem() {}
	void tick(float dt);
	void render();
	void setTTL(float ttl,float random);
	void activateMovement();
	void colorize(const Color& start,const Color& end);
	void scale(float min,float max);
	void add(ParticleModifier* m) {
		_modifiers.push_back(*m);
	}
	void start(const v2& start);
	void setEmitter(ParticleEmitter* emitter) {
		_emitter = emitter;
	}
private:
	int _max;
	int _buffer_id;
	int _texture_id;
	ParticleArray _array;
	Modifiers _modifiers;
	ParticleEmitter* _emitter;
	ModifierData _modifierData;
	ModifierDataIndices _modifierDataIndices;
	Texture _texture;
};