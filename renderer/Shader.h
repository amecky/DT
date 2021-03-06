#pragma once
#include "..\stdafx.h"
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx9math.h>
#include "ConstantBuffer.h"
#include "render_types.h"
// -------------------------------------------------------
// Shader
// -------------------------------------------------------
class Shader {

public:		
	Shader(ID3D11Device* device,ID3D11DeviceContext* deviceContext);
	virtual ~Shader();
	void initialize(const char* techName);
	bool loadPixelShader(const char* fileName, const char* techName);
	bool loadVertexShader(const char* fileName, const char* techName);
	bool loadShader(const char* fileName, const char* techName,ID3D10Blob* shaderBuffer,const char* profile);
	bool initialize(char* vsFilename, char* psFilename);
	bool setShaderParameters(int texture_id);
	bool setShaderParameters(ID3D11ShaderResourceView* shaderResourceView);
	bool setShaderParameters(ID3D11ShaderResourceView* srv1, ID3D11ShaderResourceView* srv2);
	virtual void render(int indexCount);
	void createInputLayout(const InputLayoutDefinition* definitions,int num);
protected:
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D10Blob* _vertexShaderBuffer;
	int _samplerStateIndex;
};

class DefaultShader : public Shader {

public:
	DefaultShader(ID3D11Device* device,ID3D11DeviceContext* deviceContext) : Shader(device,deviceContext) {}
	virtual ~DefaultShader() {}
	void initialize();
	void render(int indexCount);
	void setWorldMatrix(const D3DXMATRIX& world);
protected:
	int _constantBufferIndex;
	D3DXMATRIX _worldMatrix;
};

class BasicShader : public DefaultShader {

public:
	BasicShader(ID3D11Device* device, ID3D11DeviceContext* deviceContext) : DefaultShader(device, deviceContext) {}
	virtual ~BasicShader() {}
	bool create(char* vsFilename, char* psFilename);	
	void render(int indexCount);
};

class SimpleLightShader : public DefaultShader {

public:
	SimpleLightShader(ID3D11Device* device, ID3D11DeviceContext* deviceContext) : DefaultShader(device, deviceContext) {}
	virtual ~SimpleLightShader() {}
	bool create(char* vsFilename, char* psFilename);	
	void render(int indexCount);
};