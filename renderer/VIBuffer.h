#pragma once
#include "DX.h"
#include "Mesh.h"
#include "..\utils\Log.h"

class Shader;

struct BufferDescriptor {

	int type;
	int declarationID;
	int vertexSize;

};

class VIBuffer {

public:
	VIBuffer(DX* dx,int maxVertices,int vertexSize,int declaration_id);
	~VIBuffer() {}	
	void release();
	void fillBuffer(void* data, int num);	
	void render(const D3DXMATRIX* worldMatrix,int texture_id,int vertex_declaration);

	void render(Shader* shader,int texture_id);
private:
	int _max;
	DX* _dx;
	//LPDIRECT3DVERTEXBUFFER9 _buffer;
	ID3D11Buffer* _index_buffer;
	ID3D11Buffer* _buffer;  
	int _declaration_id;
	int _vertexSize;
	int _size;
};

