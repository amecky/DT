#pragma once
#include <Vector.h>
#include "..\renderer\DX.h"

struct ParticleArray {

	Vector3f* position;
	Vector3f* velocity;
	Vector3f* acceleration;
	Vector3f* normal;
	Vector2f* scale;
	float* rotation;
	Vector3f* timer;	
	float* random;
	D3DXCOLOR* color;
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
		int size = maxParticles * ( sizeof(Vector3f) * 4 + sizeof(Vector2f) + sizeof(float) + sizeof(Vector3f) + sizeof(float) + sizeof(D3DXCOLOR) + sizeof(int) + 4 * sizeof(PCTVertex));
		buffer = new char[size];
		position = (Vector3f*)(buffer);
		velocity = (Vector3f*)(position + maxParticles);
		acceleration = (Vector3f*)(velocity + maxParticles);
		normal = (Vector3f*)(acceleration + maxParticles);
		scale = (Vector2f*)(normal + maxParticles);
		timer = (Vector3f*)(scale + maxParticles);
		rotation = (float*)(timer + maxParticles);
		random = (float*)(rotation + maxParticles);
		color = (D3DXCOLOR*)(random + maxParticles);
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