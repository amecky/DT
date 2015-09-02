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

namespace assets {

	int loadTexture(const char* name);

	ID3D11ShaderResourceView* getRawTexture(int id);
}

namespace gfx {

	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

	void beginRendering(const Color& color);

	void endRendering();

	void shutdown();

	VertexIndexBuffer* createQuadBuffer(int maxQuads, int vertexSize);

	bool fillQuadBuffer(VertexIndexBuffer* buffer,void* data,int num);

	void submitBuffer(VertexIndexBuffer* buffer);

	Shader* createShader(char* vsFilename, char* psFilename);

	void renderShader(Shader* shader,int texture_id,int indexCount);

	//TextureAsset* loadTexture(const char* fileName);

	void turnZBufferOn();

	void turnZBufferOff();

	Camera* getCamera();

	void initializeBitmapFont(FontDefinition& fontDefinition,int texture_id, const Color& fillColor);

	BYTE* getImageData(ID3D11ShaderResourceView* shaderResourceView, int* nWidth, int*  nHeight);
}