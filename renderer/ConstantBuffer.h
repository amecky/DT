#pragma once
#include <assert.h>
#include "..\utils\Log.h"

class ConstantBuffer {

public:
	ConstantBuffer() : _buffer(0) {
	}
	~ConstantBuffer() {
		SAFE_RELEASE(_buffer);
		_buffer = 0;
	}
	bool create(ID3D11Device* device,int size) {
		LOG << "creating constant buffer with size: " << size;
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		HRESULT result = device->CreateBuffer(&desc, NULL, &_buffer);
		HR(result);
		if(FAILED(result)) {
			LOGE << "Failed to create constant buffer";
			return false;
		}
		return true;
	}
	template<typename T>
	void setData(ID3D11DeviceContext* deviceContext, T const& value) {
		assert( _buffer != 0 );
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        HR(deviceContext->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
        *(T*)mappedResource.pData = value;
        deviceContext->Unmap(_buffer, 0);
	}
	void setBuffer(ID3D11DeviceContext* deviceContext,int index) {
		deviceContext->VSSetConstantBuffers(index, 1, &_buffer);
	}
	void setPSBuffer(ID3D11DeviceContext* deviceContext, int index) {
		deviceContext->PSSetConstantBuffers(index, 1, &_buffer);
	}
	
private:
	ID3D11Buffer* _buffer;
};