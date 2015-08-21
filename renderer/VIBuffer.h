#pragma once
#include "DX.h"
#include "Mesh.h"
#include "..\utils\Log.h"

struct BufferDescriptor {

	int type;
	int declarationID;
	int vertexSize;

};

class VIBuffer {

public:
	VIBuffer(DX* dx,int maxVertices,int vertexSize,int declaration_id) : _dx(dx) , _max(maxVertices) , _declaration_id(declaration_id) , _vertexSize(vertexSize) {
		// create vertex buffer
		int num = _max / 4 * 6;
		LOG << "max vertices: " << _max;
		LOG << "num indices: " << num;
		dx->getDevice()->CreateVertexBuffer(_max * vertexSize,0,0,D3DPOOL_MANAGED,&_buffer,NULL);
		_dx->getDevice()->CreateIndexBuffer(num * sizeof(UINT), D3DUSAGE_WRITEONLY , D3DFMT_INDEX16, D3DPOOL_MANAGED , &_index_buffer, NULL);
		//_data = new CustomVertex[_max];
		int maxQuads = _max / 4;

		WORD* indexBuffer;
		_index_buffer->Lock(0, num * sizeof(WORD), (void**)&indexBuffer, 0);
		for (int i = 0; i < maxQuads; ++i) {
			*indexBuffer = i * 4 + 0;
			++indexBuffer;
			*indexBuffer = i * 4 + 1;
			++indexBuffer;
			*indexBuffer = i * 4 + 2;
			++indexBuffer;
			*indexBuffer = i * 4 + 2;
			++indexBuffer;
			*indexBuffer = i * 4 + 3;
			++indexBuffer;
			*indexBuffer = i * 4 + 0;
			++indexBuffer;
		}
		_index_buffer->Unlock();
	}

	~VIBuffer() {}
	
	void release() {
		_index_buffer->Release();
		_buffer->Release();
	}

	void fillBuffer(void* data, int num) {
		char* vb;
		int vc = num;
		DWORD flag = D3DLOCK_DISCARD;
		_buffer->Lock(0, vc * _vertexSize, (void**)&vb, flag);
		memcpy(vb, data, num * _vertexSize);
		_buffer->Unlock();
		_size = num;
	}
	
	void render(const D3DXMATRIX* worldMatrix,int texture_id,int vertex_declaration) {
		_dx->getDevice()->SetTransform(D3DTS_WORLD,worldMatrix);
		_dx->setVertexDeclaration(vertex_declaration);
		_dx->getDevice()->SetStreamSource(0, _buffer, 0, _vertexSize);
		_dx->getDevice()->SetIndices(_index_buffer); 
		_dx->getDevice()->SetTexture(0,_dx->getTexture(texture_id));
		int numPrim = _size / 2;
		_dx->getDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0, 0, _size, 0, numPrim);
	}

private:
	int _max;
	DX* _dx;
	LPDIRECT3DVERTEXBUFFER9 _buffer;
	LPDIRECT3DINDEXBUFFER9 _index_buffer;
	int _declaration_id;
	int _vertexSize;
	int _size;
};

