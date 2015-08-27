#pragma once
#include <d3dx9core.h>
#include <Vector.h>
#include <d3d11.h>
#include <d3dx11tex.h>

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

struct Texture {

	//IdString hashName;
	Vector4f uv;
	Vector2f dim;
	int textureID;

	Texture() : uv(0, 0, 1, 1), dim(32, 32), textureID(0) {}

	const Vector2f getUV(int idx) const {
		switch (idx) {
			case 0: return Vector2f(uv.x, uv.y); break;
			case 1: return Vector2f(uv.z, uv.y); break;
			case 2: return Vector2f(uv.z, uv.w); break;
			case 3: return Vector2f(uv.x, uv.w); break;
			default: return Vector2f(0, 0);
		}
	}
};

struct PCTVertex {

	float x,y,z;		
	float u,v;
	D3DXCOLOR color;

	PCTVertex() {}

	PCTVertex(float _x,float _y,float _z,D3DXCOLOR _color,float _u,float _v) : x(_x) , y(_y) , z(_z) , color(_color) , u(_u) , v(_v) {}

	PCTVertex(const Vector3f& p,D3DXCOLOR _color,float _u,float _v) : x(p.x) , y(p.y) , z(p.z) , color(_color) , u(_u) , v(_v) {}

	PCTVertex(float _x,float _y,float _z,float _u,float _v) : x(_x) , y(_y) , z(_z) , color(D3DCOLOR_XRGB(255,255,255)) , u(_u) , v(_v) {}
};

struct PTVertex {

	float x,y,z;		
	float u,v;

	PTVertex() {}

	PTVertex(float _x,float _y,float _z,float _u,float _v) : x(_x) , y(_y) , z(_z) , u(_u) , v(_v) {}
};

struct PCVertex {

	float x,y,z;	
	D3DXCOLOR color;

	PCVertex() {}

	PCVertex(float _x,float _y,float _z,DWORD _color) : x(_x) , y(_y) , z(_z) , color(_color) {}

	PCVertex(float _x,float _y,float _z) : x(_x) , y(_y) , z(_z) , color(D3DCOLOR_XRGB(255,255,255)){}
};

template<class T>
struct Quad {

	T v[4];

};

enum VDUSAGE {
	VDU_POSITION,VDU_NORMAL,VDU_COLOR,VDU_TEXCOORD,VDU_POSITIONT,VDU_BINORMAL,VDU_TANGENT,VDU_END
};

enum VDTYPE {
	VT_FLOAT4,VT_FLOAT3,VT_COLOR,VT_FLOAT2,VT_END
};

struct VDElement {
	VDTYPE type;
	VDUSAGE usage;
};

struct VertexDeclaration {

	int size;
	IDirect3DVertexDeclaration9* declaration;
	VertexDeclaration() : size(0) , declaration(0) {}

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

	Vector3f position;
	Vector3f extent;
	Vector3f _min;
	Vector3f _max;

	AABBox() : position(0,0,0) , extent(0,0,0) {}

	AABBox(const Vector3f& p,const Vector3f& e) : position(p) , extent(e) {
		calculateMinMax();
	}

	void translate(const Vector3f& p) {
		position = p;
		calculateMinMax();
	}

	bool collides(const AABBox& other) {
		return(_max.x > other._min.x && 
				_min.x < other._max.x &&
				_max.y > other._min.y &&
				_min.y < other._max.y &&
				_max.z > other._min.z &&
				_min.z < other._max.z);
	}

	void calculateMinMax() {
		_min.x = position.x - extent.x;
		_min.y = position.y - extent.y;
		_min.z = position.z - extent.z;
		_max.x = position.x + extent.x;
		_max.y = position.y + extent.y;
		_max.z = position.z + extent.z;
	}
};
