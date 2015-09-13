#pragma once
#include "..\..\base\BaseApp.h"
#include <vector>

const int GRID_DIM_X = 20;
const int GRID_DIM_Y = 12;
const int GRID_SIZE = 40;
const int GRID_HALF_SIZE = 20;
const int XM[4] = {1,-1,0,0};
const int YM[4] = {0,0,1,-1};
const int LD[4] = {1,0,3,2};

class PointList;

class Colors : public BaseApp {

enum GameState {
	GS_START,
	GS_RUNNING,
	GS_GAME_OVER
};

struct Piece {

	v2 position;
	int color;
};

public:
	Colors();
	~Colors();
	void loadContent();
	void tick(float dt);
	void render();
	void onChar(char ascii, unsigned int state);
	void onButton(int button, ButtonState state);
private:
	float _timer;
	void fillGrid();
	bool dropPiece(int x, int y);
	void check(int xp,int yp,int lastDir,PointList& list,bool rec);
	bool isValid(int x,int y);
	Texture _brickTextures[6];
	Texture _gridTex[3];
	Piece _pieces[GRID_DIM_X][GRID_DIM_Y];
	int _startX;
	int _startY;
	int _score;
};

