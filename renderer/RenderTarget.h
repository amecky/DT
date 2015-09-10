#pragma once
#include <d3d11.h>
#include "render_types.h"

class RenderTarget {

public:
	RenderTarget();
	//RenderTextureClass(const RenderTextureClass&);
	~RenderTarget();

	bool initialize(ID3D11Device*, int, int);
	void shutdown();

	void setRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void clearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, const Color& color);
	ID3D11ShaderResourceView* getShaderResourceView();

private:
	ID3D11Texture2D* _renderTargetTexture;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _shaderResourceView;
};