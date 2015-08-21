#include "ParticleSystem.h"
#include "..\renderer\VIBuffer.h"
#include <assert.h>

// clock wise
const float VP_ARRAY[] = {
	 0.5f , 0.5f , -0.5f , -0.5f ,// x
	-0.5f , 0.5f ,  0.5f , -0.5f  // y
};

const float UV_ARRAY[] = {
	1.0f,1.0f,0.0f,0.0f, // u
	1.0f,0.0f,0.0f,1.0f  // v
};

ParticleSystem::ParticleSystem(DX* dx,int max,int texture_id) : _dx(dx) , _max(max) , _texture_id(texture_id) {
	_array.initialize(_max);
	BufferDescriptor desc;
	desc.declarationID = 0;
	desc.vertexSize = sizeof(PCTVertex);
	desc.type = 0;
	_buffer_id = _dx->createBuffer(desc);
	_data = new PCTMeshData(_buffer_id,0,1024);
	_modifierData.allocate(1024);
}

void ParticleSystem::tick(float dt) {
	for ( size_t i = 0; i < _modifiers.size(); ++i ) {
		(*_modifiers[i])(_modifierData,_modifierDataIndices[i],&_array,dt);
	}
}

void ParticleSystem::render() {
	_dx->getDevice()->SetRenderState(D3DRS_ZENABLE, FALSE); 
	// http://www.codesampler.com/oglsrc/oglsrc_6.htm
	if ( _array.countAlive > 0 ) {
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		// Get the current view matrix:
		const D3DXMATRIX matView = _dx->getViewMatrix();
		VIBuffer* buffer = _dx->getBuffer(_buffer_id);
		_data->clear();
		for ( int i = 0; i < _array.countAlive; ++i ) {
			Vector3f p = _array.position[i];
			Vector3f s = Vector3f(_array.scale[i],1.0f);
			Quad<PCTVertex> q;
			Vector3f right = s.x * normalize(Vector3f(matView._11,matView._21,matView._31)) * 0.5f;
			Vector3f up = s.x * normalize(Vector3f(matView._12,matView._22,matView._32)) * 0.5f;
			q.v[0] = PCTVertex(p + (-right - up), _array.color[i], 0.0f, 1.0f);
			q.v[1] = PCTVertex(p + (-right + up), _array.color[i], 0.0f, 0.0f);			
			q.v[2] = PCTVertex(p + ( right + up), _array.color[i], 1.0f, 0.0f);
			q.v[3] = PCTVertex(p + ( right - up), _array.color[i], 1.0f, 1.0f);
			_data->addQuad(q);
		}
		_data->fillBuffer(buffer);
		buffer->render(&world,_texture_id,0);
	}
	_dx->getDevice()->SetRenderState(D3DRS_ZENABLE, TRUE); 
}

void ParticleSystem::start(const Vector3f& start) {
	// create instance
	assert(_emitter != 0);
	_emitter->emitt(&_array);
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

void ParticleSystem::colorize(D3DXCOLOR start,D3DXCOLOR end) {
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