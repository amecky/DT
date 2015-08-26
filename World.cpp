#include "StdAfx.h"
#include "World.h"
#include "renderer\VIBuffer.h"

World::World(DX* dx) : _dx(dx) {	
}


World::~World(void) {
	if ( _array.buffer != 0 ) {
		delete[] _array.buffer;
	}
	
}

void World::remove(MID id) {
	_array.remove(id);
}

MID World::create(const Vector3f& pos,MeshData* meshData,int shader) {
	if ( _array.total == 0 ) {
		allocate(512);
	}
	if ( _array.num + 1 >= _array.total ) {
		allocate(_array.total * 2);
	}
	MID id = _array.create(pos,meshData,shader);	
	D3DXMATRIX t;
	_array.translate(id,pos);
	return id;
}

void World::allocate(int size) {
	if ( size > _array.total ) {
		MeshArray sad;
		int sz = size * (sizeof(MeshArrayIndex) + sizeof(Vector3f) + sizeof(Vector3f) + sizeof(Vector3f) + sizeof(MID) + sizeof(D3DXMATRIX) + sizeof(MeshData*) + sizeof(int));
		sad.buffer = new char[sz];
		sad.total = size;
		sad.num = 0;
		sad.indices = (MeshArrayIndex*)(sad.buffer);
		sad.ids = (MID*)(sad.indices + size);
		sad.positions = (Vector3f*)(sad.ids + size);
		sad.scales = (Vector3f*)(sad.positions + size);
		sad.rotations = (Vector3f*)(sad.scales + size);
		sad.worlds = (D3DXMATRIX*)(sad.rotations + size);			
		sad.data = (MeshData**)(sad.worlds + size);
		sad.shaders = (int*)(sad.data + size);
		if ( _array.buffer != 0 ) {
			memcpy(sad.indices, _array.indices, _array.num * sizeof(MeshArrayIndex));
			memcpy(sad.ids, _array.ids, _array.num * sizeof(MID));
			memcpy(sad.positions, _array.positions, _array.num * sizeof(Vector3f));
			memcpy(sad.scales, _array.scales, _array.num * sizeof(Vector3f));
			memcpy(sad.rotations, _array.rotations, _array.num * sizeof(Vector3f));
			memcpy(sad.worlds, _array.worlds, _array.num * sizeof(D3DXMATRIX));
			memcpy(sad.data, _array.data, _array.num * sizeof(MeshData*));
			memcpy(sad.shaders, _array.shaders, _array.num * sizeof(int*));
			sad.free_dequeue = _array.free_dequeue;
			sad.free_enqueue = _array.free_enqueue;
			sad.num = _array.num;
			delete[] _array.buffer;
		}
		else {
			sad.clear();
		}
		_array = sad;
	}
}

void World::tick(float dt) {
	D3DXMATRIX t;
	D3DXMATRIX s;
	D3DXMATRIX rx;
	D3DXMATRIX ry;
	D3DXMATRIX rz;
	for ( int j = 0; j < _array.num; ++j ) {
		const Vector3f& v = _array.positions[j];		
		D3DXMatrixTranslation(&t, v.x,v.y,v.z);	
		const Vector3f& sz = _array.scales[j];		
		D3DXMatrixScaling(&s,sz.x,sz.y,sz.z);
		D3DXMatrixRotationX(&rx,_array.rotations[j].x);
		D3DXMatrixRotationY(&ry,_array.rotations[j].y);
		D3DXMatrixRotationZ(&rz,_array.rotations[j].z);

		_array.worlds[j] = s * rz * ry * rx * t;
	}
}

void World::render() {
	for ( int j = 0; j < _array.num; ++j ) {
		int id = _array.data[j]->getBufferID();
		VIBuffer* buffer = _dx->getBuffer(id);
		_array.data[j]->fillBuffer(buffer);
		_dx->selectTexture(_array.data[j]->getTextureID());
		_dx->setWorldTransformation(_array.worlds[j]);
		_dx->selectVertexDeclaration(_array.data[j]->getVertexDeclaration());
		if ( _array.shaders[j] == -1 ) {
			buffer->render(&_array.worlds[j],_array.data[j]->getTextureID(),_array.data[j]->getVertexDeclaration());
		}
		else {
			Shader* s = _dx->getShader(_array.shaders[j]);
			buffer->render(s,_array.data[j]->getTextureID());
		}
	}
}

void World::translate(MID id,const Vector3f& p) {
	_array.translate(id,p);
}

void World::scale(MID id,const Vector3f& s) {
	_array.scale(id,s);
}

void World::rotate(MID id,const Vector3f& r) {
	_array.rotate(id,r);
}