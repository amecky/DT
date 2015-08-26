#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx9math.h>
#include "Camera.h"
#include "render_types.h"
#include <vector>
#include <Vector.h>
#include <map>

class VIBuffer;
struct BufferDescriptor;
class Shader;

struct RenderContext {

};

class DX {

typedef std::vector<TextureAsset> TextureAssets;
typedef std::vector<VIBuffer*> BufferList;
typedef std::map<int,VertexDeclaration> DeclarationMap;
typedef std::vector<BlendState> BlendStates;
typedef std::vector<Shader*> Shaders;

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
	ID3D11Device* getDevice() const {
		return _device;
	}
	ID3D11DeviceContext* getContext() const {
		return _context;
	}

	int loadTexture(const char* name);
	LPDIRECT3DTEXTURE9 getTexture(int id) {
		return _textureAssets[id].texture;
	}
	void selectTexture(int id);


	void create(int type,VDElement* elements);
	void setVertexDeclaration(int type);
	void selectVertexDeclaration(int id);

	int createBlendState(int srcRGB, int srcAlpha, int dstRGB, int dstAlpha, bool alphaEnabled, bool separateAlpha = true);
	int createBlendState(int srcAlpha, int dstAlpha, bool alphaEnabled);
	int createBlendState(const BlendState& state);
	void changeBlendState(int id);
	void setBlendState(int id);

	void setWorldTransformation(const D3DXMATRIX& worldMatrix);

	const D3DXMATRIX& getViewMatrix() const {
		return _camera.GetViewMatrix();
	}
	void moveCamera(const Vector3f& cp) {
		_camera.setPosition(cp.x,cp.y,cp.z);
	}
	void updateCamera() {
		_camera.tick();
	}
	int loadShader(const char* fileName,const char* techName);
	void selectShader(int id);
	void prepareShader(Shader* shader, int textureID);
	Shader* getShader(int id) const;
private:
	IDXGISwapChain* _swapchain;             // the pointer to the swap chain interface
	ID3D11Device* _device;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* _context;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView* _backbuffer;
	ID3D11InputLayout* _layout;            // the pointer to the input layout
	ID3D11VertexShader* _VS;               // the pointer to the vertex shader
	ID3D11PixelShader* _PS; 

	//LPDIRECT3D9 _d3d;    // the pointer to our Direct3D interface
	//LPDIRECT3DDEVICE9 _device;    // the pointer to the device class
	D3DXMATRIX _world;
	D3DXMATRIX _wvp;
	Camera _camera;
	TextureAssets _textureAssets;
	BufferList _buffers;
	DeclarationMap _declarationMap;
	BlendStates _blendStates;
	Shaders _shaders;
	int _currentBlendState;
	int _defaultBlendState;
	int _currentShader;
	int _currentTexture;
	int _currentDeclaration;
};

