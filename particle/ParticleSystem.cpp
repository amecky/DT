#include "ParticleSystem.h"
#include "..\renderer\gfx.h"
#include "..\sprites\SpriteBatch.h"
#include <assert.h>

ParticleSystem::ParticleSystem(int max,int texture_id) : _max(max) , _texture_id(texture_id) {
	_array.initialize(_max);
	_modifierData.allocate(1024);
}

void ParticleSystem::tick(float dt) {
	for ( size_t i = 0; i < _modifiers.size(); ++i ) {
		(*_modifiers[i])(_modifierData,_modifierDataIndices[i],&_array,dt);
	}
}

void ParticleSystem::render() {
	if ( _array.countAlive > 0 ) {
		for ( int i = 0; i < _array.countAlive; ++i ) {
			sprites::draw(_array.position[i],_texture);
		}
	}
}

void ParticleSystem::start(const v2& start) {
	// create instance
	assert(_emitter != 0);
	_emitter->emitt(start,&_array);
}

void ParticleSystem::setTTL(float ttl,float random) {
	LiveOverTimeData data(ttl);
	int id = _modifierData.add(data);
	_modifierDataIndices.push_back(id);
	_modifiers.push_back(&lifeOverTime);
}

void ParticleSystem::activateMovement() {
	_modifierDataIndices.push_back(-1);
	_modifiers.push_back(&moveParticle);
}

void ParticleSystem::colorize(const Color& start,const Color& end) {
	ParticleColorData data;
	data.start = start;
	data.end = end;
	int id = _modifierData.add(data);
	_modifierDataIndices.push_back(id);
	_modifiers.push_back(&colorizeParticle);
}

void ParticleSystem::scale(float min,float max) {
	ParticleScaleData data;
	data.minSize = min;
	data.maxSize = max;
	int id = _modifierData.add(data);
	_modifierDataIndices.push_back(id);
	_modifiers.push_back(&scaleParticle);
}