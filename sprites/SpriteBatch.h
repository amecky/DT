#pragma once
#include "..\renderer\render_types.h"
#include <Vector.h>

namespace sprites {

	bool intialize(const char* textureName);

	void draw(const v2& pos, const Texture& tex, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, const D3DXCOLOR& color = D3DCOLOR_XRGB(255,255,255));

	void drawText(const char* text, int x, int y);

	void flush();

	void begin();

	void end();

	void shutdown();
}