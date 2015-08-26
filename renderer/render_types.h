#pragma once
#include <d3dx9core.h>
#include <Vector.h>
// -------------------------------------------------------
// Blending constants
// -------------------------------------------------------
const int BL_ZERO                = D3DBLEND_ZERO;
const int BL_ONE                 = D3DBLEND_ONE;
const int BL_SRC_COLOR           = D3DBLEND_SRCCOLOR;
const int BL_ONE_MINUS_SRC_COLOR = D3DBLEND_INVSRCCOLOR;
const int BL_DST_COLOR           = D3DBLEND_DESTCOLOR;
const int BL_ONE_MINUS_DST_COLOR = D3DBLEND_INVDESTCOLOR;
const int BL_SRC_ALPHA           = D3DBLEND_SRCALPHA;
const int BL_ONE_MINUS_SRC_ALPHA = D3DBLEND_INVSRCALPHA;
const int BL_DST_ALPHA           = D3DBLEND_DESTALPHA;
const int BL_ONE_MINUS_DST_ALPHA = D3DBLEND_INVDESTALPHA;
const int BL_SRC_ALPHA_SATURATE  = D3DBLEND_SRCALPHASAT;

// -------------------------------------------------------
// Blend op constants
// -------------------------------------------------------
const int BM_ADD              = D3DBLENDOP_ADD;
const int BM_SUBTRACT         = D3DBLENDOP_SUBTRACT;
const int BM_REVERSE_SUBTRACT = D3DBLENDOP_REVSUBTRACT;
const int BM_MIN              = D3DBLENDOP_MIN;
const int BM_MAX              = D3DBLENDOP_MAX;

// -------------------------------------------------------
// Alpha Func constants
// -------------------------------------------------------
const int ALPHA_GREATER = D3DCMP_GREATER;
const int ALPHA_GEQ     = D3DCMP_GREATEREQUAL;
const int ALPHA_ALWAYS  = D3DCMP_ALWAYS ;
const int ALPHA_EQ      = D3DCMP_EQUAL;
const int ALPHA_LESS    = D3DCMP_LESS;

const int COP_ADD = D3DTOP_ADD;

const int CA_TEXTURE = D3DTA_TEXTURE;
const int CA_CURRENT = D3DTA_CURRENT;
const int CA_DIFFUSE = D3DTA_DIFFUSE;

// -------------------------------------------------------
// Cull mode constants
// -------------------------------------------------------
const int CULL_NONE  = D3DCULL_NONE;
const int CULL_BACK  = D3DCULL_CCW;
const int CULL_FRONT = D3DCULL_CW;

// -------------------------------------------------------
// Fill mode constants
// -------------------------------------------------------
const int SOLID = D3DFILL_SOLID;
const int WIREFRAME = D3DFILL_WIREFRAME;

// -------------------------------------------------------
// Blend state
// -------------------------------------------------------
struct BlendState {
	int srcFactorRGB;
	int dstFactorRGB;
	int blendModeRGB;
	int srcFactorAlpha;
	int dstFactorAlpha;
	int blendModeAlpha;
	int mask;
	bool blendEnable;
	bool separateAlpha;
	int alphaRef;	
	int alphaFunc;
	int flag;
	int blendOp;
};

struct TextureAsset {
	//IdString name;
	LPDIRECT3DTEXTURE9 texture;
	int width;
	int height;
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
