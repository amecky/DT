#pragma once
#include "..\renderer\render_types.h"
#include <Vector.h>

namespace batch {

	bool initialize(const char* textureName);

	bool initialize(int textureID);

	void setTexture(int textureID);

	bool initializeFont();

	void setWorldMatrix(const D3DXMATRIX& world);

	void drawQuad(v3* vertices,const Texture& t, const Color& color = Color(255,255,255));

	void flush();

	void begin();

	void end();

	void shutdown();
}