#pragma once
#include "renderer\DX.h"
#include <d3dx9math.h>
#include "renderer\Mesh.h"
#include <assert.h>

typedef unsigned int MID;
const unsigned int INVALID_MID = UINT_MAX;

struct MeshArrayIndex {
	MID id;
	unsigned short index;
	unsigned short next;
};

struct MeshArray {

	int num;
	int total;
	MeshArrayIndex* indices;
	MID* ids;
	Vector3f* positions;
	Vector3f* scales;
	Vector3f* rotations;
	D3DXMATRIX* worlds;
	MeshData** data;
	char* buffer;

	unsigned short free_enqueue;
	unsigned short free_dequeue;

	MeshArray() : num(0) , total(0) , buffer(0) {}

	void clear() {
		if ( buffer != 0 ) {
			for ( unsigned short i = 0; i < total; ++i ) {
				indices[i].id = i;
				indices[i].next = i + 1;
			}
			num = 0;
			free_dequeue = 0;
			free_enqueue = total - 1;
		}
	}

	MID create(const Vector3f& pos,MeshData* _data) {
		MeshArrayIndex &in = indices[free_dequeue];
		free_dequeue = in.next;
		in.index = num++;
		ids[in.index] = in.id;
		data[in.index] = _data;
		positions[in.index] = pos;
		scales[in.index] = Vector3f(1.0f,1.0f,1.0f);
		rotations[in.index] = Vector3f(0.0f,0.0f,0.0f);
		return in.id;
	}

	void translate(MID id,const Vector3f& p) {
		MeshArrayIndex &in = indices[id];
		assert(in.index != USHRT_MAX);
		positions[in.index] = p;
	}

	void scale(MID id,const Vector3f& s) {
		MeshArrayIndex &in = indices[id];
		assert(in.index != USHRT_MAX);
		scales[in.index] = s;
	}

	void rotate(MID id,const Vector3f& r) {
		MeshArrayIndex &in = indices[id];
		assert(in.index != USHRT_MAX);
		rotations[in.index] = r;
	}
};

class World {

public:
	World(DX* dx);
	~World(void);
	MID create(const Vector3f& pos,MeshData* meshData);
	void render();
	void tick(float dt);
	void translate(MID id,const Vector3f& p);
	void scale(MID id,const Vector3f& p);
	void rotate(MID id,const Vector3f& r);
private:
	void allocate(int size);
	MeshArray _array;
	DX* _dx;
};
