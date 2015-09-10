#pragma once
#include <d3d11.h>
#include "render_types.h"
#include "VertexIndexBuffer.h"
#include "Shader.h"

class ScreenQuad {

public:
	ScreenQuad();
	~ScreenQuad();
	void initialize();
	void render(ID3D11ShaderResourceView* shaderResourceView);
	void render(Shader* shader,ID3D11ShaderResourceView* shaderResourceView);
	void render(Shader* shader, ID3D11ShaderResourceView* srv1, ID3D11ShaderResourceView* srv2);
private:
	PCTVertex _vertices[4];
	VertexIndexBuffer* _buffer;
	Shader* _shader;
	int _constantBufferIndex;
	int _blendState;
};