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
private:
	PCTVertex _vertices[4];
	VertexIndexBuffer* _buffer;
	DefaultShader* _shader;
	int _constantBufferIndex;
	int _blendState;
};