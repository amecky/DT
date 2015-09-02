#include "..\..\StdAfx.h"
#include "Slingy.h"
#include "..\..\renderer\render_types.h"
#include "..\..\renderer\Mesh.h"
#include "..\..\sprites\SpriteBatch.h"
#include "..\..\utils\Log.h"
#include "..\..\math\mathutils.h"

BaseApp *app = new Slingy();

Slingy::Slingy() {
	_settings.tickCamera = false;
	_settings.screenSizeX = 800;
	_settings.screenSizeY = 600;
	_settings.clearColor = Color(0,0,0);
	//_CrtSetBreakAlloc(339);
}


Slingy::~Slingy() {
	sprites::shutdown();
}


void Slingy::loadContent() {
	sprites::intialize("content\\array.png");
	_ballTexture = math::buildTexture(Rect(0,38,24,24),512.0f,512.0f,true);
	_tailTexture = math::buildTexture(Rect(0,0,22,22),512.0f,512.0f,true);

	FILE *fp = fopen("l1.txt", "rb");
	char* text;
	if (fp) {
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* text = (char*)malloc(sizeof(char) * (size + 1));
		fread (text, 1, size, fp);
		fclose(fp);
		LOG << "size: " << size;
		for ( int i = 0; i < 30; ++i ) {
			char* line = text + i * 41;
			for ( int j = 0; j < 40; ++j ) {
				if ( line[j] == 'x' ) {
					addWall(v2(20 + j*20,10 + (29-i)*20),20,20);
				}
				else if ( line[j] == 'E' ) {
					_exit.position = v2(20 + j*20,10 + (29-i)*20);
					_exit.texture = math::buildTexture(30,105,24,24,512.0f,512.0f,true);
				}
				else if ( line[j] == 'S' ) {
					_startPos = v2(20 + j*20,10 + (29-i)*20);
				}
				else if ( line[j] == '*' ) {
					Star s;
					s.position = v2(20 + j*20,10 + (29-i)*20);
					s.texture = math::buildTexture(0,75,26,26,512.0f,512.0f,true);
					_stars.push_back(s);
				}
			}
		}
		delete[] text;
	}

	_ball.position = _startPos;
	_ball.aabBox = AABBox(_ball.position,v2(15,15));
	_ball.angle = 0.0f;
	for ( int i = 0; i < 10; ++i ) {
		Tail t;
		t.position = _ball.position;
		t.position.x -= 22.0f;
		t.aabBox = AABBox(t.position,v2(10,10));
		t.angle = 0.0f;
		_tails.push_back(t);
	}

	

}

void Slingy::drawLine(const v2& start,const v2& end,int thickness) {
	v2 diff = end - start;	
	int l = length(diff);
	diff *= 0.5f;
	float angle = math::get_target_angle(end,start);
	sprites::draw(start + diff,math::buildTexture(Rect(275,0,l,thickness),512.0f,512.0f,true),angle);
}

void Slingy::addWall(const v2& p,int width,int height) {
	Wall w;
	w.position = p;
	w.texture = math::buildTexture(Rect(30,75,width,height),512.0f,512.0f,true);
	w.aabBox = AABBox(p,v2(width/2,height/2));
	_walls.push_back(w);
}

void Slingy::resetBall() {
	_ball.sticky = true;
	_ball.angle = 0.0f;
	_ball.position = _startPos;
	_ball.aabBox.translate(_ball.position);
	for ( size_t i = 0; i < _tails.size(); ++i ) {
		_tails[i].position = v2(_startPos.x - 22.0f,_startPos.y);
	}
}

void Slingy::moveTail(float dt) {
	v2 parent = _ball.position;
	for ( size_t i = 0; i < _tails.size(); ++i ) {
		v2 diff = parent - _tails[i].position;
		if ( sqr_length(diff) > 22.0f ) {
			float tooFar = length(diff) - 22.0f;
			v2 translate = normalize(diff) * tooFar;
			_tails[i].position += translate;
			_tails[i].aabBox.translate(_tails[i].position);
			_tails[i].angle = math::get_target_angle(parent,_tails[i].position);
		}
		parent = _tails[i].position;
	}
}

void Slingy::moveBall(float dt) {
	_ball.position += _ball.velocity * dt;
	if ( _ball.position.x < 10.0f || _ball.position.x > 790.0f ) {
		resetBall();
	}
	if ( _ball.position.y < 10.0f || _ball.position.y > 590.0f ) {
		resetBall();
	}
	_ball.aabBox.translate(_ball.position);
}

void Slingy::tick(float dt) {

	_timer += dt;

	if ( !_ball.sticky ) {
		float da = 0.0f;
		if ( GetAsyncKeyState('A') != 0 ) {
			da = 1.0f;
		}
		if ( GetAsyncKeyState('D') != 0 ) {
			da = -1.0f;
		}
		if ( da != 0.0f ) {
			_ball.angle += da * dt * 2.0f;
			_ball.velocity = math::get_radial_velocity(_ball.angle,80.0f);
		}

		moveBall(dt);

		moveTail(dt);

		for ( size_t i = 0; i < _walls.size(); ++i ) {
			if ( _walls[i].aabBox.collides(_ball.aabBox)) {
				resetBall();
			}
		}

	}
}

void Slingy::render() {
	sprites::begin();

	for ( size_t i = 0; i < _walls.size(); ++i ) {
		sprites::draw(_walls[i].position,_walls[i].texture);
	}

	sprites::draw(_ball.position, _ballTexture, _ball.angle, 1.0f, 1.0f, Color(192,0,0));

	for ( size_t i = 0; i < _tails.size(); ++i ) {
		sprites::draw(_tails[i].position, _tailTexture, _tails[i].angle, 1.0f, 1.0f, Color(192, 0, 192));
	}
	sprites::draw(_exit.position,_exit.texture);
	for ( size_t i = 0; i < _stars.size(); ++i ) {
		sprites::draw(_stars[i].position,_stars[i].texture);
	}
	sprites::drawText("Hello World", 100, 20,Color(192,0,0));
	sprites::end();
}

void Slingy::onChar(char ascii, unsigned int state) {
	if ( ascii == '1' ) {
		if ( _ball.sticky ) {
			_ball.sticky = false;
			_ball.velocity = math::get_radial_velocity(_ball.angle,150.0f);
		}
	}
}