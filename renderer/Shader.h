#pragma once
#include "..\stdafx.h"
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx9math.h>
// -------------------------------------------------------
// Shader
// -------------------------------------------------------
class Shader {

struct MatrixBufferType	{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

public:		
	Shader();
	~Shader() {}
	void initialize(const char* techName);
	bool loadShader(const char* fxName, const char* techName);
	bool initialize(ID3D11Device* device,char* vsFilename, char* psFilename);
	bool setShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
											 D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void render(ID3D11DeviceContext* deviceContext, int indexCount);
private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11Buffer* _matrixBuffer;
	ID3D11SamplerState* _sampleState;
	ID3D11BlendState* _alphaBlendState;
};