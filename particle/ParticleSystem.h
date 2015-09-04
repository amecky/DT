#pragma once
#include "ParticleArray.h"
#include "..\renderer\Mesh.h"
#include "ParticleModifier.h"
#include "ParticleEmitter.h"
#include "..\utils\stringutils.h"
#include <vector>

struct ParticleSystemInstance {

	ParticleArray array;
	ParticleEmitter* emitter;

};

struct ModifierDefinition {

	char* name;
	ParticleModifier function;

};

struct Modifier {

	int dataIndex;
	int definitionIndex;

};

struct ParticleChannel {

	char* name;
	IdString hash;
	int outputMapping;

	ParticleChannel(char* _name,int _mapping) : name(_name) , outputMapping(_mapping) {
		hash = string::murmur_hash(_name);
	}

};

// http://www.gdcvault.com/play/1020176/Scripting-Particles-Getting-Native-Speed

class ParticleSystem {

typedef std::vector<Modifier> Modifiers;

public:
	ParticleSystem(int max,int texture_id);
	~ParticleSystem() {
		delete _emitter;
	}
	void setTexture(const Texture& texture) {
		_texture = texture;
	}

	void initialize(ParticleChannel* channels,int length);

	void tick(float dt);
	void render();
	template<class T>
	void addModifier(const char* name,const T& data) {
		Modifier m;
		m.dataIndex = _modifierData.add(data);
		m.definitionIndex = findFunctionIndex(name);
		if ( m.definitionIndex != -1 ) {
			_modifiers.push_back(m);
		}
	}
	void addModifier(const char* name) {
		Modifier m;
		m.dataIndex = -1;
		m.definitionIndex = findFunctionIndex(name);
		if ( m.definitionIndex != -1 ) {
			_modifiers.push_back(m);
		}
	}
	void addGenerator(const char* name) {
		_emitter->addGenerator(name);
	}
	template<class T>
	void addGenerator(const char* name,const T& data) {
		_emitter->addGenerator(name,data);
	}
	void start(const v2& start);
	void setEmitter(ParticleEmitter* emitter) {
		_emitter = emitter;
	}
private:
	int findFunctionIndex(const char* name);
	int _max;
	int _buffer_id;
	int _texture_id;
	ParticleArray _array;
	Modifiers _modifiers;
	ParticleEmitter* _emitter;
	ParticleDataBuffer _modifierData;
	Texture _texture;
	
	ParticleBlob _blob;
	int _channelMapping[4];
};