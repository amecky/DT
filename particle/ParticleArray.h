#pragma once
#include <Vector.h>
#include "..\renderer\gfx.h"
#include "..\renderer\render_types.h"

struct ParticleDataBuffer {

	char* buffer;
	int index;
	int num;
	int entries[64];

	ParticleDataBuffer() : buffer(0), index(0), num(0) {}

	~ParticleDataBuffer() {
		if (buffer != 0) {
			delete[] buffer;
		}
	}

	void allocate(int size) {
		buffer = new char[size];
	}

	template<class T>
	const bool get(int id, T* t, int size) const {
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

struct ParticleArray {

	v2* position;
	v2* velocity;
	v2* acceleration;
	v2* normal;
	v2* scale;
	float* rotation;
	v3* timer;	
	float* random;
	Color* color;
	int* type;
	PCTVertex* vertices;
	char* buffer;

	int count;
	int countAlive;

	ParticleArray() : count(0) , countAlive(0) , buffer(0) {}

	~ParticleArray() {		
		if ( buffer != 0 ) {
			delete[] buffer;
		}
	}

	void initialize(unsigned int maxParticles) {
		int size = maxParticles * ( sizeof(v2) * 4 + sizeof(v2) + sizeof(float) + sizeof(v3) + sizeof(float) + sizeof(Color) + sizeof(int) + 4 * sizeof(PCTVertex));
		buffer = new char[size];
		position = (v2*)(buffer);
		velocity = (v2*)(position + maxParticles);
		acceleration = (v2*)(velocity + maxParticles);
		normal = (v2*)(acceleration + maxParticles);
		scale = (Vector2f*)(normal + maxParticles);
		timer = (Vector3f*)(scale + maxParticles);
		rotation = (float*)(timer + maxParticles);
		random = (float*)(rotation + maxParticles);
		color = (Color*)(random + maxParticles);
		type = (int*)(color + maxParticles);
		vertices = (PCTVertex*)(type + maxParticles);
		count = maxParticles;
		countAlive = 0;
	}

	void swapData(int a, int b) {
		if ( a != b ) {
			position[a] = position[b];
			velocity[a] = velocity[b];
			acceleration[a] = acceleration[b];
			normal[a] = normal[b];
			scale[a] = scale[b];
			rotation[a] = rotation[b];
			timer[a] = timer[b];
			random[a] = random[b];
			color[a] = color[b];
			type[a] = type[b];
		}
	}

	void kill(int id) {
		if (countAlive > 0) {
			swapData(id, countAlive - 1);
			--countAlive;
		}
	}

	void wake(int id) {
		if (countAlive < count)	{
			swapData(id, countAlive);
			++countAlive;
		}
	}   

};