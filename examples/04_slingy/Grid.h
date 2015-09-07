#pragma once
#include "..\..\renderer\render_types.h"

const int GRID_X = 24;
const int GRID_Y = 18;
const int GRID_SIZE = 40;

class Grid {

struct GridEntry {

	float factor;
	float timer;
	bool fading;

	GridEntry() : factor(0.5f) , timer(0.0f) , fading(false) {}

};

public:
	Grid();
	~Grid();
	void render();
	void setHighlight(const v2& p);
	void tick(float dt);
private:
	void setFactor(int x, int y, float factor);
	GridEntry _entries[GRID_X][GRID_Y];
	Texture _gridTexture;
	Texture _leftTexture;
	Texture _topTexture;
	v2 _highlight;
	float _timer;
};

