#include "shader.h"
#include "..\utils\StringUtils.h"
#include "..\utils\Log.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "gfx.h"

Shader::Shader(ID3D11Device* device,ID3D11DeviceContext* deviceContext) : _device(device) , _deviceContext(deviceContext) {
	_vertexShaderBuffer = 0;
}

Shader::~Shader() {
	SAFE_RELEASE(_vertexShaderBuffer);
	SAFE_RELEASE(_vertexShader);
	SAFE_RELEASE(_pixelShader);
}

void Shader::initialize(const char* techName) {
	LOGC("renderer") << "initializing shader using tech: " << techName;
	LOGC("renderer") << "Shader finally loaded";

}

bool Shader::loadPixelShader(const char* fileName, const char* techName) {
	// Compile the vertex shader code.
	LOG << "Loading shader: " << fileName << " using tech: " << techName;
	ID3D10Blob* errorMessage;
	ID3D10Blob* pixelShaderBuffer = 0;
	char buffer[256];
	sprintf_s(buffer,256,"content\\effects\\%s",fileName);
	HRESULT result = D3DX11CompileFromFile(buffer, NULL, NULL, techName, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result)) {
		if(errorMessage) {
			LOGE << (char*)errorMessage->GetBufferPointer();			
		}
		else {
			LOGE << "Missing shader file: " << buffer;
		}
		return false;
	}	
	// Create the pixel shader from the buffer.
    result = _device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if(FAILED(result)) {
		return false;
	}

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
	return true;
}

bool Shader::loadVertexShader(const char* fileName, const char* techName) {
	// Compile the pixel shader code.
	LOG << "Loading shader: " << fileName << " using tech: " << techName;
	char buffer[256];
	sprintf_s(buffer,256,"content\\effects\\%s",fileName);
	ID3D10Blob* errorMessage;
	HRESULT result = D3DX11CompileFromFile(buffer, NULL, NULL, techName, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result)) {
		if(errorMessage) {
			LOGE << (char*)errorMessage->GetBufferPointer();			
		}
		else {
			LOGE << "Missing shader file: " << buffer;
		}
		return false;
	}	
	result = _device->CreateVertexShader(_vertexShaderBuffer->GetBufferPointer(), _vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if(FAILED(result)) {
		return false;
	}
	return true;
}

bool Shader::loadShader(const char* fileName, const char* techName,ID3D10Blob* shaderBuffer,const char* profile) {
	// Compile the vertex shader code.
	LOG << "Loading shader: " << fileName << " using tech: " << techName;
	char buffer[256];
	sprintf_s(buffer,256,"content\\effects\\%s",fileName);
	ID3D10Blob* errorMessage;
	HRESULT result = D3DX11CompileFromFile(buffer, NULL, NULL, techName, profile, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &shaderBuffer, &errorMessage, NULL);
	if(FAILED(result)) {
		if(errorMessage) {
			LOGE << (char*)errorMessage->GetBufferPointer();			
		}
		else {
			LOGE << "Missing shader file: " << buffer;
		}
		return false;
	}	
	return true;
}



bool Shader::initialize(char* vsFilename, char* psFilename) {

	if ( !loadVertexShader(vsFilename,"TextureVertexShader") ) {
		return false;
	}
	if ( !loadPixelShader(psFilename,"TexturePixelShader") ) {
		return false;
	}
	_samplerStateIndex = gfx::createSamplerState(TAM_WRAP);
	return true;
}

void Shader::createInputLayout(const InputLayoutDefinition* definitions,int num) {
	D3D11_INPUT_ELEMENT_DESC* layout = new D3D11_INPUT_ELEMENT_DESC[num];
	bool first = true;
	for ( int i = 0; i < num; ++i ) {
		layout[i].SemanticName = definitions[i].name;
		layout[i].SemanticIndex = 0;
		layout[i].Format = definitions[i].format;
		layout[i].InputSlot = 0;
		if ( first ) {
			layout[i].AlignedByteOffset = 0;
			first = false;
		}
		else {
			layout[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		}
		layout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout[i].InstanceDataStepRate = 0;
	}
	// Create the vertex input layout.
	HR(_device->CreateInputLayout(layout, num, _vertexShaderBuffer->GetBufferPointer(), _vertexShaderBuffer->GetBufferSize(),&_layout));
	delete[] layout;
}

bool Shader::setShaderParameters(ID3D11ShaderResourceView* shaderResourceView) {
	_deviceContext->PSSetShaderResources(0, 1, &shaderResourceView);
	return true;
}

bool Shader::setShaderParameters(int texture_id) {
	ID3D11ShaderResourceView* texture = assets::getRawTexture(texture_id);
	_deviceContext->PSSetShaderResources(0, 1, &texture);
	return true;
}


void Shader::render(int indexCount) {
	// Set the vertex input layout.
	_deviceContext->IASetInputLayout(_layout);
    // Set the vertex and pixel shaders that will be used to render this triangle.
    _deviceContext->VSSetShader(_vertexShader, NULL, 0);
    _deviceContext->PSSetShader(_pixelShader, NULL, 0);
	// Set the sampler state in the pixel shader.
	gfx::setSamplerState(_samplerStateIndex);
	// Render the triangle.
	_deviceContext->DrawIndexed(indexCount, 0, 0);
	return;
}




void DefaultShader::initialize() {
	Shader::initialize("basic_ptc.vs","basic_ptc.ps");
	createInputLayout(PTC_LAYOUT,3);
	_constantBufferIndex = gfx::createConstantBuffer(sizeof(ConstantMatrixBuffer));
}

void DefaultShader::render(int indexCount) {
	ConstantMatrixBuffer buffer;
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);
	D3DXMATRIX viewMatrix = gfx::getViewMatrix();
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMATRIX projectionMatrix = gfx::getProjectionMatrix();
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	buffer.world = world;
	buffer.view = viewMatrix;
	buffer.projection = projectionMatrix;
	ConstantBuffer* cb = gfx::getConstantBuffer(_constantBufferIndex);	
	cb->setData(gfx::getDeviceContext(),buffer);
	cb->setBuffer(gfx::getDeviceContext(),0);
	Shader::render(indexCount);
}