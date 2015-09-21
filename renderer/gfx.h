#pragma once
#include "..\stdafx.h"
#include <d3dx9math.h>
#include "Camera.h"
#include <d3d11.h>
#include <d3dx11tex.h>
#include "Vector.h"
#include "render_types.h"
#include "ConstantBuffer.h"

class VertexIndexBuffer;
class Shader;
class DefaultShader;
class BasicShader;
class SimpleLightShader;

namespace assets {

	int loadTexture(const char* name);

	int createTexture(int size,const Color& fillColor);

	ID3D11ShaderResourceView* getRawTexture(int id);
}

namespace debug {

	void reset();

	void addSprites(int cnt);

	void log();
}

namespace gfx {

	int createRenderTarget();

	ID3D11ShaderResourceView* getRenderTargetSRV(int index);

	void clearRenderTarget(int id,const Color& color);

	void setRenderTarget(int id);

	void drawRenderTarget(int id);

	void restoreBackBuffer();

	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

	void beginRendering(const Color& color);

	void endRendering();

	void shutdown();

	int createConstantBuffer(int elementSize);

	ConstantBuffer* getConstantBuffer(int index);

	ID3D11DeviceContext* getDeviceContext();

	const D3DXMATRIX& getViewMatrix();

	const D3DXMATRIX& getProjectionMatrix();

	VertexIndexBuffer* createQuadBuffer(int maxQuads, int vertexSize);

	bool fillQuadBuffer(VertexIndexBuffer* buffer,void* data,int num);

	void submitBuffer(VertexIndexBuffer* buffer);

	Shader* createShader(char* vsFilename, char* psFilename);

	BasicShader* createBasicShader(char* vsFilename, char* psFilename);

	SimpleLightShader* createSimpleLightShader(char* vsFilename, char* psFilename);

	DefaultShader* getDefaultShader();

	void renderShader(Shader* shader,int texture_id,int indexCount);

	void renderShader(Shader* shader,ID3D11ShaderResourceView* shaderResourceView,int indexCount);

	void renderShader(Shader* shader, ID3D11ShaderResourceView* srv1, ID3D11ShaderResourceView* srv2, int indexCount);

	void attachInputLayout(Shader* shader,const InputLayoutDefinition* definitions,int num);

	const v2& getScreenSize();

	const v2& getScreenCenter();

	void setScreenCenter(const v2& c);

	//TextureAsset* loadTexture(const char* fileName);

	int createSamplerState(TextureAddressMode mode);

	void setSamplerState(int index);

	void turnZBufferOn();

	void turnZBufferOff();

	Camera* getCamera();

	void initializeBitmapFont(FontDefinition& fontDefinition,int texture_id, const Color& fillColor);

	BYTE* getImageData(ID3D11ShaderResourceView* shaderResourceView, int* nWidth, int*  nHeight);

	int createBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,D3D11_BLEND srcBlendAlpha, D3D11_BLEND destBlendAlpha);

	int createBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend);

	void setBlendState(int index);

	void setMousePos(int x, int y);

	v2 getMousePos();
}