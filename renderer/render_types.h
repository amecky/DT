#pragma once
//#include <d3dx9core.h>
#include <Vector.h>
#include <d3d11.h>
#include <d3dx11tex.h>
#include "..\stdafx.h"
#include "..\math\mathutils.h"

struct TextureAsset {
	//IdString name;
	ID3D11ShaderResourceView* texture;
	int width;
	int height;

	TextureAsset() : texture(0) , width(0) , height(0) {}

	~TextureAsset() {
		SAFE_RELEASE(texture);
	}
};

struct Color {

	float r;
	float g;
	float b;
	float a;

	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	Color(BYTE _r, BYTE _g, BYTE _b, BYTE _a) {
		r = static_cast<float>(_r) / 255.0f;
		g = static_cast<float>(_g) / 255.0f;
		b = static_cast<float>(_b) / 255.0f;
		a = static_cast<float>(_a) / 255.0f;
	}
	Color(int _r, int _g, int _b) {
		r = static_cast<float>(_r) / 255.0f;
		g = static_cast<float>(_g) / 255.0f;
		b = static_cast<float>(_b) / 255.0f;
		a = 1.0f;
	}
	Color(int _r, int _g, int _b, int _a) {
		r = static_cast<float>(_r) / 255.0f;
		g = static_cast<float>(_g) / 255.0f;
		b = static_cast<float>(_b) / 255.0f;
		a = static_cast<float>(_a) / 255.0f;
	}

	bool operator == (const Color& other) {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	bool operator != (const Color& other) {
		return r != other.r || g != other.g || b != other.b || a != other.a;
	}

	float* operator() () {
		return &r;
	}

	operator FLOAT* () {
		return &r;
	}

    operator CONST FLOAT* () const {
		return &r;
	}

};

struct Texture {

	//IdString hashName;
	v4 uv;
	v2 dim;
	int textureID;

	Texture() : uv(0, 0, 1, 1), dim(32, 32), textureID(0) {}

	const v2 getUV(int idx) const {
		switch (idx) {
			case 0: return v2(uv.x, uv.y); break;
			case 1: return v2(uv.z, uv.y); break;
			case 2: return v2(uv.z, uv.w); break;
			case 3: return v2(uv.x, uv.w); break;
			default: return v2(0, 0);
		}
	}
};

// -------------------------------------------------------
// Character definition
// -------------------------------------------------------
struct CharDef {

	int ascii;
	int startX;
	int startY;
	int width;
	Texture texture;

	CharDef() : ascii(-1), startX(0), startY(0), width(0) {}

};

// -------------------------------------------------------
// Font definition
// -------------------------------------------------------
struct FontDefinition {

	int startChar;// " : "32",
	int endChar;// " : "128",
	int charHeight;// " : "14",
	int gridHeight;// " : "21",
	int startX;// " : "0",
	int startY;// " : "300",
	int width;// " : "405",
	int height;// " : "168",
	int padding;// " : "6",
	int textureSize;// " : "1024"
	CharDef definitions[255];

	void addChar(uint32 ascii, int startX, int startY, int width) {
		CharDef cd;
		cd.ascii = ascii;
		cd.startX = startX;
		cd.startY = startY;
		cd.width = width;
		cd.texture = math::buildTexture(static_cast<float>(startY), static_cast<float>(startX), static_cast<float>(width), static_cast<float>(charHeight), static_cast<float>(textureSize), static_cast<float>(textureSize), true);
		definitions[ascii] = cd;
	}

};

struct PCTVertex {

	float x,y,z;		
	float u,v;
	Color color;

	PCTVertex() {}

	PCTVertex(float _x,float _y,float _z,Color _color,float _u,float _v) : x(_x) , y(_y) , z(_z) , color(_color) , u(_u) , v(_v) {}

	PCTVertex(const Vector3f& p,Color _color,float _u,float _v) : x(p.x) , y(p.y) , z(p.z) , color(_color) , u(_u) , v(_v) {}

	PCTVertex(float _x,float _y,float _z,float _u,float _v) : x(_x) , y(_y) , z(_z) , color(1.0f,1.0f,1.0f,1.0f) , u(_u) , v(_v) {}
};

struct PTVertex {

	float x,y,z;		
	float u,v;

	PTVertex() {}

	PTVertex(float _x,float _y,float _z,float _u,float _v) : x(_x) , y(_y) , z(_z) , u(_u) , v(_v) {}
};

struct PCVertex {

	float x,y,z;	
	Color color;

	PCVertex() {}

	PCVertex(float _x,float _y,float _z,const Color& _color) : x(_x) , y(_y) , z(_z) , color(_color) {}

	PCVertex(float _x,float _y,float _z) : x(_x) , y(_y) , z(_z) , color(1.0f,1.0f,1.0f,1.0f){}
};

template<class T>
struct Quad {

	T v[4];

};

struct Rect {
	
	float top,left,bottom,right;


	Rect() : top(0.0f) , left(0.0f) , bottom(0.0f) , right(0.0f) {}

	Rect(float _top,float _left,float width,float height) : top(_top) , left(_left) {
		bottom = _top + height;
		right = _left + width;
	}

	Rect(int _top,int _left,int _width,int _height) {
		top = static_cast<float>(_top);
		left = static_cast<float>(_left);
		bottom = top + static_cast<float>(_height);
		right = left + static_cast<float>(_width);
	}

	Rect(const Rect& rect) {
		top = rect.top;
		left = rect.left;
		bottom = rect.bottom;
		right = rect.right;
	}

	float Rect::width() const {
		return right - left;
	}

	float Rect::height() const {
		return bottom - top;
	}
};

struct AABBox {

	v2 position;
	v2 extent;
	v2 _min;
	v2 _max;

	AABBox() : position(0,0) , extent(0,0) {}

	AABBox(const v2& p,const v2& e) : position(p) , extent(e) {
		calculateMinMax();
	}

	void translate(const v2& p) {
		position = p;
		calculateMinMax();
	}

	bool collides(const AABBox& other) {
		return(_max.x > other._min.x && 
				_min.x < other._max.x &&
				_max.y > other._min.y &&
				_min.y < other._max.y);
	}

	void calculateMinMax() {
		_min.x = position.x - extent.x;
		_min.y = position.y - extent.y;
		_max.x = position.x + extent.x;
		_max.y = position.y + extent.y;
	}
};
