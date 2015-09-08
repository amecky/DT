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
	Shader();
	~Shader();
	void initialize(const char* techName);
	bool loadPixelShader(ID3D11Device* device,const char* fileName, const char* techName);
	bool loadVertexShader(ID3D11Device* device,const char* fileName, const char* techName);
	bool loadShader(const char* fileName, const char* techName,ID3D10Blob* shaderBuffer,const char* profile);
	bool initialize(ID3D11Device* device,char* vsFilename, char* psFilename);
	bool setShaderParameters(ID3D11DeviceContext* deviceContext,int texture_id);
	void render(ID3D11DeviceContext* deviceContext, int indexCount);
	void createInputLayout(ID3D11Device* device,const InputLayoutDefinition* definitions,int num);
private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D10Blob* _vertexShaderBuffer;
	int _samplerStateIndex;
};