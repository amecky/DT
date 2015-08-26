#include "..\StdAfx.h"
#include "VIBuffer.h"
#include "..\utils\Log.h"
#include "Mesh.h"
#include "Shader.h"
#include <assert.h>

VIBuffer::VIBuffer(DX* dx,int maxVertices,int vertexSize,int declaration_id) : _dx(dx) , _max(maxVertices) , _declaration_id(declaration_id) , _vertexSize(vertexSize) {

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = vertexSize * maxVertices;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	_dx->getDevice()->CreateBuffer(&bd, NULL, &_buffer);       // create the buffer


	// create static index buffer

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	int num = _max / 4 * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * num;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	int maxQuads = maxVertices / 4;
	LOG << "num: " << num << " maxQuads: " << maxQuads;
	WORD* indexBuffer = new WORD[num];
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
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indexBuffer;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	assert(_dx->getDevice() != 0);
	HR(_dx->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_index_buffer));
	//delete[] indexBuffer;
}

	
	
void VIBuffer::release() {
	_index_buffer->Release();
	_buffer->Release();
}

void VIBuffer::fillBuffer(void* data, int num) {
	D3D11_MAPPED_SUBRESOURCE ms;
	_dx->getContext()->Map(_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, data, _vertexSize * num);//sizeof(OurVertices));                // copy the data
	_dx->getContext()->Unmap(_buffer, NULL);    

	//char* vb;
	//int vc = num;
	//DWORD flag = D3DLOCK_DISCARD;
	//_buffer->Lock(0, vc * _vertexSize, (void**)&vb, flag);
	//memcpy(vb, data, num * _vertexSize);
	//_buffer->Unlock();
	_size = num;
}
	
void VIBuffer::render(const D3DXMATRIX* worldMatrix,int texture_id,int vertex_declaration) {
	// select which vertex buffer to display
    UINT stride = _vertexSize;
	UINT offset = 0;
		
	_dx->getContext()->IASetVertexBuffers(0, 1, &_buffer, &stride, &offset);
	_dx->getContext()->IASetIndexBuffer(_index_buffer, DXGI_FORMAT_R16_UINT, 0);
      
	// select which primtive type we are using    
	_dx->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int numPrim = _size / 4 * 6;
        // draw the vertex buffer to the back buffer
	//_dx->getContext()->Draw(numPrim, 0);

	_dx->getContext()->DrawIndexed(numPrim, 0, 0);

	/*
	//_dx->getDevice()->SetTransform(D3DTS_WORLD,worldMatrix);
	//_dx->setVertexDeclaration(vertex_declaration);
	_dx->getDevice()->SetStreamSource(0, _buffer, 0, _vertexSize);
	_dx->getDevice()->SetIndices(_index_buffer); 
	//_dx->getDevice()->SetTexture(0,_dx->getTexture(texture_id));
	int numPrim = _size / 2;
	_dx->getDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0, 0, _size, 0, numPrim);
	*/
}

void VIBuffer::render(Shader* shader,int texture_id) {
	/*
	_dx->getDevice()->SetStreamSource(0, _buffer, 0, _vertexSize);
	_dx->getDevice()->SetIndices(_index_buffer); 
	int numPrim = _size / 2;
	uint32 numPasses = shader->start();	
	for (UINT p = 0; p < numPasses; ++p) {
		shader->beginPass(p);		
		_dx->prepareShader(shader, texture_id);
		_dx->getDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0, 0, _size, 0, numPrim);
		shader->endPass();
	}

	shader->end();	
	*/
}