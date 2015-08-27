#pragma once
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
	int* shaders;
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

	MID create(const Vector3f& pos,MeshData* _data,int shader = -1) {
		MeshArrayIndex &in = indices[free_dequeue];
		free_dequeue = in.next;
		in.index = num++;
		ids[in.index] = in.id;
		data[in.index] = _data;
		positions[in.index] = pos;
		scales[in.index] = Vector3f(1.0f,1.0f,1.0f);
		rotations[in.index] = Vector3f(0.0f,0.0f,0.0f);
		shaders[in.index] = shader;
		return in.id;
	}

	void remove(MID id) {
		MeshArrayIndex &in = indices[id];
		assert(in.index != USHRT_MAX);
		MID currentID = ids[num - 1];
		MeshArrayIndex& next = indices[currentID];
		ids[in.index] = ids[next.index];
		data[in.index] = data[next.index];
		positions[in.index] = positions[next.index];
		scales[in.index] = scales[next.index];		
		rotations[in.index] = rotations[next.index];		
		shaders[in.index] = shaders[next.index];	
		--num;
		indices[currentID].index = in.index;
		in.index = USHRT_MAX;
		indices[free_enqueue].next = id;
		free_enqueue = id;
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
	World();
	~World(void);
	MID create(const Vector3f& pos,MeshData* meshData,int shader = -1);
	void remove(MID id);
	void render();
	void tick(float dt);
	void translate(MID id,const Vector3f& p);
	void scale(MID id,const Vector3f& p);
	void rotate(MID id,const Vector3f& r);
private:
	void allocate(int size);
	MeshArray _array;
};

