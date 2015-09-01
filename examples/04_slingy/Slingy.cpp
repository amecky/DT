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
	//_settings.clearColor = D3DCOLOR_XRGB(0,0,0);
	//_CrtSetBreakAlloc(339);
}


Slingy::~Slingy() {
	sprites::shutdown();
}


void Slingy::loadContent() {
	sprites::intialize("content\\array.png");
	_ballTexture = math::buildTexture(Rect(0,200,30,30),512.0f,512.0f,true);
	_tailTexture = math::buildTexture(Rect(30,200,24,15),512.0f,512.0f,true);
	_ball.position = v2(400,300);
	_ball.aabBox = AABBox(_ball.position,v2(15,15));
	_ball.angle = 0.0f;
	for ( int i = 0; i < 6; ++i ) {
		Tail t;
		t.position = _ball.position;
		t.position.x -= 26.0f * i;
		t.aabBox = AABBox(t.position,v2(10,10));
		t.angle = 0.0f;
		_tails.push_back(t);
	}
	/*
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
		for ( int i = 0; i < 15; ++i ) {
			char* line = text + i * 21;
			for ( int j = 0; j < 20; ++j ) {
				if ( line[j] == 'x' ) {
					addWall(v2(20 + j*40,20 + i*40),40,40);
				}
			}
		}
		delete[] text;
	}
	*/
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
	w.texture = math::buildTexture(Rect(275,0,width,height),512.0f,512.0f,true);
	w.aabBox = AABBox(p,v2(width/2,height/2));
	_walls.push_back(w);
}

void Slingy::resetBall() {
	_ball.sticky = true;
	_ball.angle = 0.0f;
	_ball.position = v2(400,300);
	_ball.aabBox.translate(_ball.position);
}

void Slingy::moveTail(float dt) {
	v2 parent = _ball.position;
	for ( size_t i = 0; i < _tails.size(); ++i ) {
		v2 diff = parent - _tails[i].position;
		if ( sqr_length(diff) > 20.0f ) {
			float tooFar = length(diff) - 20.0f;
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
			_ball.velocity = math::get_radial_velocity(_ball.angle,150.0f);
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

	drawLine(v2(100,100),v2(300,200),3);

	sprites::draw(v2(100,100),_tailTexture);

	sprites::draw(v2(300,200),_tailTexture);

	for ( size_t i = 0; i < _walls.size(); ++i ) {
		sprites::draw(_walls[i].position,_walls[i].texture);
	}
	sprites::draw(_ball.position,_ballTexture,_ball.angle);
	for ( size_t i = 0; i < _tails.size(); ++i ) {
		sprites::draw(_tails[i].position,_tailTexture,_tails[i].angle);
	}
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