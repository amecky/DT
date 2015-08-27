#pragma once
#include <d3dcommon.h>
#include <d3d11.h>

class VertexIndexBuffer {

public:
	VertexIndexBuffer();
	~VertexIndexBuffer();
	void create(ID3D11Device* device,int maxVertices,int vertexSize);
	void fillBuffer(ID3D11DeviceContext* deviceContext,void* data, int num);	
	void submit(ID3D11DeviceContext* deviceContext);
private:
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	int _indexCount;
	int _maxVertices;
	int _vertexSize;
};