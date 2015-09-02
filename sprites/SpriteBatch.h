#pragma once
#include "..\renderer\render_types.h"
#include <Vector.h>

namespace sprites {

	bool intialize(const char* textureName);

	void draw(const v2& pos, const Texture& tex, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, const Color& color = Color(255,255,255));

	void drawText(const char* text, int x, int y,const Color& color = Color(255,255,255,255),int padding = 4);

	v2 calculateTextSize(const char* text,int padding = 4);

	void flush();

	void begin();

	void end();

	void shutdown();
}