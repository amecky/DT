#include "ScreenQuad.h"
#include "..\stdafx.h"
#include "gfx.h"

ScreenQuad::ScreenQuad() {
	
}

ScreenQuad::~ScreenQuad() {
	delete _buffer;
}

void ScreenQuad::initialize() {
	_buffer = gfx::createQuadBuffer(4,sizeof(PCTVertex));
	_shader = gfx::getDefaultShader();
	_blendState = gfx::createBlendState(D3D11_BLEND_ONE ,D3D11_BLEND_INV_SRC_ALPHA,D3D11_BLEND_ZERO,D3D11_BLEND_ZERO);
	const v2& sz = gfx::getScreenSize();
	float hx = sz.x * 0.5f;
	float hy = sz.y * 0.5f;
	float hz = 0.0f;
	_vertices[0] = PCTVertex(-hx, -hy, 0.0f, Color(255, 255, 255),0.0f,1.0f);
	_vertices[1] = PCTVertex(-hx,  hy, 0.0f, Color(255, 255, 255),0.0f,0.0f);
	_vertices[2] = PCTVertex( hx,  hy, 0.0f, Color(255, 255, 255),1.0f,0.0f);
	_vertices[3] = PCTVertex( hx, -hy, 0.0f, Color(255, 255, 255),1.0f,1.0f);
}

void ScreenQuad::render(ID3D11ShaderResourceView* shaderResourceView) {
	gfx::fillQuadBuffer(_buffer,_vertices,4);
	gfx::turnZBufferOff();
	gfx::submitBuffer(_buffer);
	gfx::setBlendState(_blendState);
	int indexCount = 6;
	gfx::renderShader(_shader,shaderResourceView,indexCount);
	debug::addSprites(1);
}

void ScreenQuad::render(Shader* shader,ID3D11ShaderResourceView* shaderResourceView) {
	gfx::fillQuadBuffer(_buffer, _vertices, 4);
	gfx::turnZBufferOff();
	gfx::submitBuffer(_buffer);
	gfx::setBlendState(_blendState);
	int indexCount = 6;
	gfx::renderShader(shader, shaderResourceView, indexCount);
	debug::addSprites(1);
}

void ScreenQuad::render(Shader* shader, ID3D11ShaderResourceView* srv1, ID3D11ShaderResourceView* srv2) {
	gfx::fillQuadBuffer(_buffer, _vertices, 4);
	gfx::turnZBufferOff();
	gfx::submitBuffer(_buffer);
	gfx::setBlendState(_blendState);
	int indexCount = 6;
	gfx::renderShader(shader, srv1, srv2, indexCount);
	debug::addSprites(1);
}