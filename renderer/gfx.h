#pragma once
#include "..\stdafx.h"
#include <d3dx9math.h>

class VertexIndexBuffer;
class Shader;
struct TextureAsset;

namespace gfx {

	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

	void beginRendering(const D3DXCOLOR& color);

	void endRendering();

	void shutdown();

	VertexIndexBuffer* createQuadBuffer(int maxQuads, int vertexSize);

	bool fillQuadBuffer(VertexIndexBuffer* buffer,void* data,int num);

	void submitBuffer(VertexIndexBuffer* buffer);

	Shader* createShader(char* vsFilename, char* psFilename);

	void renderShader(Shader* shader,TextureAsset* asset);

	TextureAsset* loadTexture(char* fileName);

	void turnZBufferOn();

	void turnZBufferOff();
}