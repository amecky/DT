#pragma once
#include "..\stdafx.h"
#include <d3dx9math.h>
#include "Camera.h"
#include <d3d11.h>
#include <d3dx11tex.h>

class VertexIndexBuffer;
class Shader;
struct TextureAsset;
struct FontDefinition;
struct Color;

namespace gfx {

	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

	void beginRendering(const D3DXCOLOR& color);

	void endRendering();

	void shutdown();

	VertexIndexBuffer* createQuadBuffer(int maxQuads, int vertexSize);

	bool fillQuadBuffer(VertexIndexBuffer* buffer,void* data,int num);

	void submitBuffer(VertexIndexBuffer* buffer);

	Shader* createShader(char* vsFilename, char* psFilename);

	void renderShader(Shader* shader,TextureAsset* asset,int indexCount);

	TextureAsset* loadTexture(const char* fileName);

	void turnZBufferOn();

	void turnZBufferOff();

	Camera* getCamera();

	void initializeBitmapFont(FontDefinition& fontDefinition,TextureAsset* textureAsset, const Color& fillColor);

	BYTE* getImageData(ID3D11ShaderResourceView* shaderResourceView, int* nWidth, int*  nHeight);
}