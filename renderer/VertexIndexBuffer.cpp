#include "VertexIndexBuffer.h"
#include <assert.h>
#include "..\stdafx.h"

VertexIndexBuffer::VertexIndexBuffer() {
}

VertexIndexBuffer::~VertexIndexBuffer() {
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_vertexBuffer);
}

void VertexIndexBuffer::create(ID3D11Device* device,int maxVertices,int vertexSize) {
	_vertexSize = vertexSize;
	_indexCount = maxVertices / 4 * 6;
	int maxQuads = maxVertices / 4;

	D3D11_BUFFER_DESC vertexBufferDesc;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = _vertexSize * maxVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	
	// Now create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, NULL, &_vertexBuffer);
	if(FAILED(result)) {
		//return false;
		
	}

	assert(_vertexBuffer != 0);

	// Load the index array with data.
	unsigned long* indices = new unsigned long[_indexCount];		
	for (int i = 0; i < maxQuads; ++i) {
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4 + 0;		
	}


	D3D11_BUFFER_DESC indexBufferDesc;		
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if(FAILED(result)) {
		// FIXME: clean up
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	//delete [] vertices;
	//vertices = 0;

	delete[] indices;
	indices = 0;
}

void VertexIndexBuffer::fillBuffer(ID3D11DeviceContext* deviceContext, void* data, int num) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// Lock the vertex buffer so it can be written to.
	HRESULT result = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result)) {
		//return false;
	}
	// Copy the data into the vertex buffer.
	memcpy(mappedResource.pData, data, _vertexSize * num);

	// Unlock the vertex buffer.
	deviceContext->Unmap(_vertexBuffer, 0);
}

void VertexIndexBuffer::submit(ID3D11DeviceContext* deviceContext) {
	unsigned int offset = 0;
	unsigned int stride = _vertexSize;
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}