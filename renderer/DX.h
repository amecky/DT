#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include "Camera.h"
#include "render_types.h"
#include <vector>
#include <Vector.h>
#include <map>

class VIBuffer;
struct BufferDescriptor;

class DX {

typedef std::vector<TextureAsset> TextureAssets;
typedef std::vector<VIBuffer*> BufferList;
typedef std::map<int,VertexDeclaration> DeclarationMap;
typedef std::vector<BlendState> BlendStates;

public:
	DX(void);
	~DX(void);
	void init(HWND hWnd,int sizeX,int sizeY);
	void begin(const D3DXCOLOR& clearColor);
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
	void moveCamera(const Vector3f& cp) {
		_camera.setPosition(cp.x,cp.y,cp.z);
	}
	void updateCamera() {
		_camera.tick();
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

