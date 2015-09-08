#include "shader.h"
#include "..\utils\StringUtils.h"
#include "..\utils\Log.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "gfx.h"

Shader::Shader() {
	_vertexShaderBuffer = 0;
}

Shader::~Shader() {
	_vertexShaderBuffer->Release();
	_vertexShaderBuffer = 0;
}

void Shader::initialize(const char* techName) {
	LOGC("renderer") << "initializing shader using tech: " << techName;
	LOGC("renderer") << "Shader finally loaded";

}

bool Shader::loadPixelShader(ID3D11Device* device,const char* fileName, const char* techName) {
	// Compile the vertex shader code.
	LOG << "Loading shader: " << fileName << " using tech: " << techName;
	ID3D10Blob* errorMessage;
	ID3D10Blob* pixelShaderBuffer = 0;
	HRESULT result = D3DX11CompileFromFile(fileName, NULL, NULL, techName, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result)) {
		if(errorMessage) {
			LOGE << (char*)errorMessage->GetBufferPointer();			
		}
		else {
			LOGE << "Missing shader file: " << fileName;
		}
		return false;
	}	
	// Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if(FAILED(result)) {
		return false;
	}

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
	return true;
}

bool Shader::loadVertexShader(ID3D11Device* device,const char* fileName, const char* techName) {
	// Compile the pixel shader code.
	LOG << "Loading shader: " << fileName << " using tech: " << techName;
	ID3D10Blob* errorMessage;
	HRESULT result = D3DX11CompileFromFile(fileName, NULL, NULL, techName, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result)) {
		if(errorMessage) {
			LOGE << (char*)errorMessage->GetBufferPointer();			
		}
		else {
			LOGE << "Missing shader file: " << fileName;
		}
		return false;
	}	
	result = device->CreateVertexShader(_vertexShaderBuffer->GetBufferPointer(), _vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if(FAILED(result)) {
		return false;
	}
	return true;
}

bool Shader::loadShader(const char* fileName, const char* techName,ID3D10Blob* shaderBuffer,const char* profile) {
	// Compile the vertex shader code.
	LOG << "Loading shader: " << fileName << " using tech: " << techName;
	ID3D10Blob* errorMessage;
	HRESULT result = D3DX11CompileFromFile(fileName, NULL, NULL, techName, profile, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &shaderBuffer, &errorMessage, NULL);
	if(FAILED(result)) {
		if(errorMessage) {
			LOGE << (char*)errorMessage->GetBufferPointer();			
		}
		else {
			LOGE << "Missing shader file: " << fileName;
		}
		return false;
	}	
	return true;
}



bool Shader::initialize(ID3D11Device* device,char* vsFilename, char* psFilename) {

	if ( !loadVertexShader(device,vsFilename,"TextureVertexShader") ) {
		return false;
	}
	if ( !loadPixelShader(device,psFilename,"TexturePixelShader") ) {
		return false;
	}
	LOG << "All shaders loaded";
	//HRESULT result;
	//ID3D10Blob* errorMessage;
	
	
	/*
	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	
	pixelShaderBuffer = 0;

    // Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
								   &_vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result)) {
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage) {
			LOGE << errorMessage;
			//OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else {
			//MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
			LOGE << "Missing shader file: " << vsFilename;
		}

		return false;
	}

    // Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
								   &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result)) {
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage) {
			LOGE << errorMessage;
			//OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else {
			//MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
			LOGE << "Missing shader file: " << vsFilename;
		}
		return false;
	}
	*/
    // Create the vertex shader from the buffer.
    

    
	_samplerStateIndex = gfx::createSamplerState(TAM_WRAP);
	return true;
}

void Shader::createInputLayout(ID3D11Device* device,const InputLayoutDefinition* definitions,int num) {
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
	HR(device->CreateInputLayout(layout, num, _vertexShaderBuffer->GetBufferPointer(), _vertexShaderBuffer->GetBufferSize(),&_layout));
	delete[] layout;
}

bool Shader::setShaderParameters(ID3D11DeviceContext* deviceContext,int texture_id) {
	ID3D11ShaderResourceView* texture = assets::getRawTexture(texture_id);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	return true;
}


void Shader::render(ID3D11DeviceContext* deviceContext, int indexCount) {
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(_layout);
    // Set the vertex and pixel shaders that will be used to render this triangle.
    deviceContext->VSSetShader(_vertexShader, NULL, 0);
    deviceContext->PSSetShader(_pixelShader, NULL, 0);
	// Set the sampler state in the pixel shader.
	gfx::setSamplerState(_samplerStateIndex);
	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
	return;
}