#include "Grid.h"
#include "..\..\sprites\SpriteBatch.h"
#include <d3dx9.h>

Grid::Grid() {
	_gridTexture = math::buildTexture(Rect(30,  75, 40, 40), 512.0f, 512.0f, true);
	_leftTexture = math::buildTexture(Rect(30, 141,  9, 30), 512.0f, 512.0f, true);
	_topTexture = math::buildTexture(Rect( 60, 150, 30,  9), 512.0f, 512.0f, true);
	_highlight = v2(0, 0);
	_timer = 0.0f;
}


Grid::~Grid() {
}

void Grid::tick(float dt) {
	_timer += dt;
	for (int y = 0; y < GRID_Y; ++y) {
		for (int x = 0; x < GRID_X; ++x) {
			GridEntry& entry = _entries[x][y];
			if (entry.fading) {
				entry.timer += dt;
				float norm = entry.timer / 2.0f;
				entry.factor = 0.5f + sin(norm * D3DX_PI) * 0.5f;
				if (norm >= 1.0f) {
					entry.fading = false;
				}
			}
		}
	}
}

void Grid::render() {
	
	for (int y = 0; y < GRID_Y; ++y) {
		for (int x = 0; x < GRID_X; ++x) {
			int alpha = _entries[x][y].factor * 255;
			sprites::draw(v2(x * GRID_SIZE + 45, y * GRID_SIZE + 45), _gridTexture, 0.0f, 1.0f, 1.0f, Color(48, 0, 0, alpha));
		}
	}
	/*
	int border = 230 + sin(_timer*2.0f) * 25;
	for (int y = 0; y < GRID_Y; ++y) {
		sprites::draw(v2(24, y * GRID_SIZE + 45), _leftTexture, 0.0f, 1.0f, 1.0f, Color(48, 0, 0, border));
		sprites::draw(v2(786, y * GRID_SIZE + 45), _leftTexture, 0.0f, 1.0f, 1.0f, Color(48, 0, 0, border));
	}
	for (int x = 0; x < GRID_X; ++x) {
		sprites::draw(v2(x * GRID_SIZE + 45, 24), _topTexture, 0.0f, 1.0f, 1.0f, Color(48, 0, 0, border));
		sprites::draw(v2(x * GRID_SIZE + 45, 576), _topTexture, 0.0f, 1.0f, 1.0f, Color(48, 0, 0, border));
	}
	*/
}

void Grid::setFactor(int x, int y, float factor) {
	if (x >= 0 && x < GRID_X && y >= 0 && y < GRID_Y) {
		GridEntry& entry = _entries[x][y];
		if (entry.fading) {
			if (entry.timer > 0.5f) {
				entry.timer = 0.5f;
			}
		}
		else {
			entry.factor = 0.5f;
			entry.timer = 0.0f;
			entry.fading = true;
		}
	}
}
void Grid::setHighlight(const v2& p) {
	int xp = (p.x - GRID_SIZE + 25) / GRID_SIZE;
	int yp = (p.y - GRID_SIZE + 25) / GRID_SIZE;
	if (xp >= 0 && xp < GRID_X && yp >= 0 && yp < GRID_Y) {
		for (int y = yp - 1; y < yp + 2; ++y) {
			for (int x = xp - 1; x < xp + 2; ++x) {
				if (x == xp && y == yp) {
					setFactor(x, y, 1.0f);
				}
				else {
					setFactor(x, y, 0.8f);
				}
			}
		}
		_highlight = v2(xp, yp);
	}
}