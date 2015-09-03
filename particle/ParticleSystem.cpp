#include "ParticleSystem.h"
#include "..\renderer\gfx.h"
#include "..\sprites\SpriteBatch.h"
#include <assert.h>

const int NUM_DEFINITIONS = 4;

const ModifierDefinition DEFINITIONS[] = {
	{"move",&moveParticle},
	{"lifecycle",&lifeOverTime},
	{"scale",&scaleParticle},
	{"colorize",&colorizeParticle},
};

ParticleSystem::ParticleSystem(int max,int texture_id) : _max(max) , _texture_id(texture_id) {
	_array.initialize(_max);
	_modifierData.allocate(1024);
	_emitter = new ParticleEmitter;
}

int ParticleSystem::findFunctionIndex(const char* name) {
	for ( int i =0; i < NUM_DEFINITIONS; ++i ) {
		if ( strcmp(name,DEFINITIONS[i].name) == 0 ) {
			return i;
		}
	}
	return -1;
}

void ParticleSystem::tick(float dt) {
	for ( size_t i = 0; i < _modifiers.size(); ++i ) {
		const Modifier& m = _modifiers[i];
		(*DEFINITIONS[m.definitionIndex].function)(_modifierData,m.dataIndex,&_array,dt);
	}
}

void ParticleSystem::render() {
	if ( _array.countAlive > 0 ) {
		for ( int i = 0; i < _array.countAlive; ++i ) {
			sprites::draw(_array.position[i],_texture,_array.rotation[i],_array.scale[i].x,_array.scale[i].y,_array.color[i]);
		}
	}
}

void ParticleSystem::start(const v2& start) {
	// create instance
	assert(_emitter != 0);
	_emitter->emitt(start,&_array);
}
