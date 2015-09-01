#pragma once
#include "..\renderer\render_types.h"
#include <Vector.h>

namespace sprites {

	bool intialize(const char* textureName);

	void draw(const v2& pos,const Texture& tex, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);

	void flush();

	void begin();

	void end();

	void shutdown();
}