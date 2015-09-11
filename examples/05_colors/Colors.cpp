#include "..\..\StdAfx.h"
#include "Colors.h"
#include "..\..\renderer\render_types.h"
#include "..\..\sprites\SpriteBatch.h"
#include "..\..\utils\Log.h"
#include "..\..\math\mathutils.h"
#include "..\..\renderer\gfx.h"


BaseApp *app = new Colors();

struct Point {
	int x;
	int y;
};

class PointList {

typedef std::vector<Point> Points;

public:
	PointList() {}
	~PointList() {}
	void add(int x,int y) {
		if ( !contains(x,y) ) {
			Point p;
			p.x = x;
			p.y = y;
			_points.push_back(p);
		}
	}
	bool contains(int x,int y) const {
		if ( x >= 0 && x < GRID_DIM_X && y >= 0 && y < GRID_DIM_Y) {
			for ( size_t i = 0; i < _points.size(); ++i ) {
				if ( _points[i].x == x && _points[i].y == y ) {
					return true;
				}
			}
		}
		return false;
	}
	const Point& get(int index) const {
		return _points[index];
	}
	const size_t size() const {
		return _points.size();
	}
private:
	Points _points;
};
Colors::Colors() {
	_settings.tickCamera = false;
	_settings.screenSizeX = 1024;
	_settings.screenSizeY = 768;
	_settings.clearColor = Color(0,0,0);
	//_CrtSetBreakAlloc(339);
	_startX = (1024 - (GRID_DIM_X * GRID_SIZE - GRID_HALF_SIZE)) / 2;
	_startY = ( 768 - (GRID_DIM_Y * GRID_SIZE - GRID_HALF_SIZE)) / 2;
}


Colors::~Colors() {
	sprites::shutdown();
}


void Colors::loadContent() {
	sprites::intialize("colors");
	for ( int i = 0; i < 6; ++i ) {
		_brickTextures[i] = math::buildTexture(Rect(1, i * GRID_SIZE + 1, 40, 40), 512.0f, 512.0f, true);
	}
	fillGrid();
}

void Colors::tick(float dt) {
}

void Colors::render() {
	sprites::begin();
	v2 pos(0,0);
	for ( int y = 0; y < GRID_DIM_Y; ++y ) {
		for ( int x = 0; x < GRID_DIM_X; ++x ) {
			const Piece& p = _pieces[x][y];
			pos.x = _startX + x * GRID_SIZE;
			pos.y = _startY + y * GRID_SIZE;
			if ( p.color != -1 ) {
				sprites::draw(pos,_brickTextures[p.color]);
			}
		}
	}
	sprites::end();
}

void Colors::fillGrid() {
	for ( int y = 0; y < GRID_DIM_Y; ++y ) {
		for ( int x = 0; x < GRID_DIM_X; ++x ) {
			_pieces[x][y].color = math::random(0,5);
		}
	}
}

void Colors::onButton(int button, ButtonState state) {
	if ( button == 0 ) {
		v2 mp = gfx::getMousePos();
		int gx = (mp.x - _startX + GRID_HALF_SIZE) / GRID_SIZE;
		int gy = (mp.y - _startY + GRID_HALF_SIZE) / GRID_SIZE;
		LOG << "grid pos: " << gx << " " << gy;
		PointList list;
		check(gx,gy,-1,list,true);
		list.add(gx,gy);
		// check if we are part of the list
		LOG << "list: " << list.size();
		for ( size_t i = 0; i < list.size(); ++i ) {
			const Point& p = list.get(i);
			LOG << i << " : " << p.x << " " << p.y;
			_pieces[p.x][p.y].color = -1;
		}
	}
}

void Colors::onChar(char ascii, unsigned int state) {
	if ( ascii == '1' ) {
	}
}

bool Colors::isValid(int x,int y) {
	if ( x < 0 ) {
		return false;
	}
	if ( y < 0 ) {
		return false;
	}
	if ( x >= GRID_DIM_X ) {
		return false;
	}
	if ( y >= GRID_DIM_Y ) {
		return false;
	}
	return true;
}

// -------------------------------------------------------------
// check recursivley to detect matching pieces
// -------------------------------------------------------------
void Colors::check(int xp,int yp,int lastDir,PointList& list,bool rec) {
	if ( isValid(xp,yp) ) {
		Piece& t = _pieces[xp][yp];
		int color = t.color;
		for ( int i = 0 ; i < 4; ++i ) {
			if ( i != lastDir ) {
				int sx = xp + XM[i];
				int sy = yp + YM[i];
				if ( isValid(sx,sy)) {
					Piece& nt = _pieces[sx][sy];
					int nc = nt.color;
					while ( color == nc && color != -1 ) {												
						bool recheck = !list.contains(sx,sy);
						list.add(sx,sy);
						if ( recheck && rec ) {
							check(sx,sy,LD[i],list,rec);
						}												
						sx += XM[i];
						sy += YM[i];
						if ( isValid(sx,sy)) {
							Piece& npe = _pieces[sx][sy];
							nc = npe.color;
						}
						else {
							nc = -1;
						}						
					}
				}
			}
		}
	}
}