#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include "Camera.h"
#include "render_types.h"
#include <vector>
#include <Vector.h>
#include <map>

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

class VIBuffer;
struct BufferDescriptor;

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

class DX {

typedef std::vector<TextureAsset> TextureAssets;
typedef std::vector<VIBuffer*> BufferList;
typedef std::map<int,VertexDeclaration> DeclarationMap;
typedef std::vector<BlendState> BlendStates;

public:
	DX(void);
	~DX(void);
	void init(HWND hWnd);
	void begin();
	void end();
	void render();
	void shutdown();
	int createBuffer(const BufferDescriptor& desc);
	VIBuffer* getBuffer(int idx) const;
	LPDIRECT3DDEVICE9 getDevice() const {
		return _device;
	}
	int loadTexture(const char* name);
	LPDIRECT3DTEXTURE9 getTexture(int id) {
		return _textureAssets[id].texture;
	}
	void create(int type,VDElement* elements);
	void setVertexDeclaration(int type);
	int createBlendState(int srcRGB, int srcAlpha, int dstRGB, int dstAlpha, bool alphaEnabled, bool separateAlpha = true);
	int createBlendState(int srcAlpha, int dstAlpha, bool alphaEnabled);
	int createBlendState(const BlendState& state);
	void changeBlendState(int id);
	void setBlendState(int id);
	const D3DXMATRIX& getViewMatrix() const {
		return *_camera.GetViewMatrix();
	}
private:
	LPDIRECT3D9 _d3d;    // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 _device;    // the pointer to the device class
	D3DXMATRIX _world;
	Camera _camera;
	TextureAssets _textureAssets;
	BufferList _buffers;
	DeclarationMap _declarationMap;
	BlendStates _blendStates;
	int _currentBlendState;
	int _defaultBlendState;
};

