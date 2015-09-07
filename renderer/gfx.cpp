#include "gfx.h"
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <assert.h>
#include <vector>
#include "VertexIndexBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "render_types.h"
#include "..\utils\Log.h"
#include "..\utils\stringutils.h"

// -------------------------------------------------
// RenderContext
// -------------------------------------------------
struct RenderContext {

	bool vsync_enabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	ID3D11DepthStencilState* depthDisabledStencilState;
	Camera camera;
};

// -------------------------------------------------
// AssetContext
// -------------------------------------------------
struct AssetContext {

	std::vector<TextureAsset> textures;

};

static RenderContext* ctx = 0;

static AssetContext* assetCtx = 0;

// -------------------------------------------------
// assets
// -------------------------------------------------
namespace assets {

	void initialize() {
		assetCtx = new AssetContext;
	}

	// -------------------------------------------------
	// find texture
	// -------------------------------------------------
	int findTexture(const char* name) {
		IdString hash = string::murmur_hash(name);
		for (size_t i = 0; i < assetCtx->textures.size(); ++i) {
			if (assetCtx->textures[i].hashName == hash) {
				return i;
			}
		}
		return -1;
	}

	// -------------------------------------------------
	// load texture
	// -------------------------------------------------
	int loadTexture(const char* name) {
		LOGC("assets") << "load texture: " << name;
		int idx = findTexture(name);
		if (idx != -1) {
			LOGC("assets") << "found already existing texture - returning " << idx;
			return idx;
		}
		TextureAsset asset;
		char fileName[256];
		sprintf(fileName, "content\\%s.png", name);
		LOGC("assets") << "loading texture from file: " << fileName;
		HRESULT result = D3DX11CreateShaderResourceViewFromFile(ctx->device, fileName, NULL, NULL, &asset.texture, NULL);
		if (FAILED(result)) {
			return -1;
		}
		assetCtx->textures.push_back(asset);
		return assetCtx->textures.size() - 1;
	}

	ID3D11ShaderResourceView* getRawTexture(int id) {
		assert(id >= 0 && id < assetCtx->textures.size());
		return assetCtx->textures[id].texture;
	}

	void shutdown() {
		LOGC("assets") << "shutting down context";
		for (size_t i = 0; i < assetCtx->textures.size(); ++i) {
			SAFE_RELEASE(assetCtx->textures[i].texture);
		}
		delete assetCtx;
		assetCtx = 0;
	}
}

// -------------------------------------------------
// gfx
// -------------------------------------------------
namespace gfx {

	Camera* getCamera() {
		return &ctx->camera;
	}

	void turnZBufferOn() {
		ctx->deviceContext->OMSetDepthStencilState(ctx->depthStencilState, 1);	
	}


	void turnZBufferOff() {
		ctx->deviceContext->OMSetDepthStencilState(ctx->depthDisabledStencilState, 1);
	}
	
	BYTE* getImageData(ID3D11ShaderResourceView* shaderResourceView,int* nWidth,int*  nHeight) {
		ID3D11Resource* resource = NULL;;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		if (shaderResourceView)	{
			shaderResourceView->GetResource(&resource);
			shaderResourceView->GetDesc(&srvDesc);
		}
		else {
			return NULL;
		}
		ID3D11Texture2D* tex = (ID3D11Texture2D*)resource;
		if (tex) {
			D3D11_TEXTURE2D_DESC description;
			tex->GetDesc(&description);
			description.BindFlags = 0;
			description.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			description.Usage = D3D11_USAGE_STAGING;

			ID3D11Texture2D* texTemp = NULL;

			HRESULT hr = ctx->device->CreateTexture2D(&description, NULL, &texTemp);
			if (FAILED(hr))	{
				if (texTemp) {
					texTemp->Release();
					texTemp = NULL;
				}
				return NULL;
			}
			ctx->deviceContext->CopyResource(texTemp, tex);

			D3D11_MAPPED_SUBRESOURCE  mapped;
			unsigned int subresource = 0;
			hr = ctx->deviceContext->Map(texTemp, 0, D3D11_MAP_READ, 0, &mapped);
			if (FAILED(hr)) {
				texTemp->Release();
				texTemp = NULL;
				return NULL;
			}

			*nWidth = description.Width;
			*nHeight = description.Height;
			const int pitch = mapped.RowPitch;
			BYTE* source = (BYTE*)(mapped.pData);
			BYTE* dest = new BYTE[(*nWidth)*(*nHeight) * 4];
			BYTE* destTemp = dest;
			for (int i = 0; i < *nHeight; ++i)
			{
				memcpy(destTemp, source, *nWidth * 4);
				source += pitch;
				destTemp += *nWidth * 4;
			}
			ctx->deviceContext->Unmap(texTemp, 0);
			return dest;
		}
		else
			return NULL;
	}

	Color getColor(BYTE* data, int x, int y, int width) {
		int idx = x * 4 + y * width * 4;
		Color c((int)data[idx],(int)data[idx + 1],(int)data[idx + 2],(int)data[idx + 3]);
		return c;
	}

	void initializeBitmapFont(FontDefinition& fontDefinition, int texture_id, const Color& fillColor) {
		int width = 0;
		int height = 0;
		ID3D11ShaderResourceView* texture = assets::getRawTexture(texture_id);
		assert(texture != 0);
		BYTE* data = getImageData(texture, &width, &height);
		int x = fontDefinition.startX + fontDefinition.padding - 1;
		int y = fontDefinition.startY + fontDefinition.padding;
		uint32 ascii = fontDefinition.startChar;
		Color c = getColor(data, x, y, width);
		bool running = true;
		bool isChar = false;
		int charStartedX = 0;
		int charStartedY = 0;
		int charCount = 0;
		while (running) {
			++x;
			if (x > (fontDefinition.startX + fontDefinition.width)) {
				x = fontDefinition.startX + fontDefinition.padding - 1;
				y += fontDefinition.padding + fontDefinition.gridHeight;// - 1;
				isChar = false;
				charCount = 0;
			}
			if (y >= (fontDefinition.startY + fontDefinition.height)) {
				running = false;
			}
			if (y >= height) {
				running = false;
			}
			if (running) {
				c = getColor(data, x, y, width);
				if ( c != fillColor && !isChar) {
					isChar = true;
					charStartedX = x;
					charStartedY = y;
				}
				else if (c == fillColor && isChar) {
					isChar = false;
					int width = x - charStartedX - 1;
					++charCount;
					fontDefinition.addChar(ascii, charStartedX + 1, charStartedY, width);
					++ascii;
				}

			}
		}
		delete[] data;
	}

	void renderShader(Shader* shader,int texture_id,int indexCount) {
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		if (shader->setShaderParameters(ctx->deviceContext, world, ctx->camera.GetViewMatrix(), ctx->camera.GetProjectionMatrix(), texture_id)) {
			shader->render(ctx->deviceContext,indexCount);
		}
		else {
			LOG << "cannot set shader parameter";
		}
	}

	Shader* createShader(char* vsFilename, char* psFilename) {
		Shader* s = new Shader;
		s->initialize(ctx->device,vsFilename,psFilename);
		return s;
	}

	void submitBuffer(VertexIndexBuffer* buffer) {
		buffer->submit(ctx->deviceContext);
	}

	bool fillQuadBuffer(VertexIndexBuffer* buffer,void* data,int num) {
		buffer->fillBuffer(ctx->deviceContext,data,num);
		return true;
	}
	// ----------------------------------------------
	// create buffer
	// ----------------------------------------------
	VertexIndexBuffer* createQuadBuffer(int maxQuads,int vertexSize) {
		assert(ctx != 0);
		VertexIndexBuffer* vib = new VertexIndexBuffer();
		vib->create(ctx->device, maxQuads * 4, vertexSize);
		return vib;		
	}

	// ----------------------------------------------
	// begin rendering
	// ----------------------------------------------
	void beginRendering(const Color& color) {
		assert(ctx != 0);
		D3DXCOLOR clr;
		ctx->deviceContext->ClearRenderTargetView(ctx->renderTargetView, color);
		ctx->deviceContext->ClearDepthStencilView(ctx->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	// ----------------------------------------------
	// end rendering
	// ----------------------------------------------
	void endRendering() {
		assert(ctx != 0);
		if(ctx->vsync_enabled) {
			ctx->swapChain->Present(1, 0);
		}
		else {
			ctx->swapChain->Present(0, 0);
		}
	}

	// ----------------------------------------------
	// shutdown
	// ----------------------------------------------
	void shutdown() {
		
		assets::shutdown();

		assert(ctx != 0);
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		if(ctx->swapChain) {
			ctx->swapChain->SetFullscreenState(false, NULL);
		}
		SAFE_RELEASE(ctx->depthDisabledStencilState);
		SAFE_RELEASE(ctx->rasterState);
		SAFE_RELEASE(ctx->depthStencilView);
		SAFE_RELEASE(ctx->depthStencilState);
		SAFE_RELEASE(ctx->depthStencilBuffer);
		SAFE_RELEASE(ctx->renderTargetView);
		SAFE_RELEASE(ctx->deviceContext);
		SAFE_RELEASE(ctx->device);
		SAFE_RELEASE(ctx->swapChain);
		delete ctx;
		ctx = 0;
	}

	// ----------------------------------------------
	// initialize
	// ----------------------------------------------
	bool initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear) {
		ctx = new RenderContext;	
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator, denominator, stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* backBufferPtr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_VIEWPORT viewport;
		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;


		// Store the vsync setting.
		ctx->vsync_enabled = vsync;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if(FAILED(result)) {
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if(FAILED(result)) {
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if(FAILED(result)) {
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if(FAILED(result)) {
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if(!displayModeList) {
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if(FAILED(result)) {
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for(i=0; i<numModes; i++) {
			if(displayModeList[i].Width == (unsigned int)screenWidth) {
				if(displayModeList[i].Height == (unsigned int)screenHeight) {
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if(FAILED(result)) {
			return false;
		}

		// Store the dedicated video card memory in megabytes.
		ctx->videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		error = wcstombs_s(&stringLength, ctx->videoCardDescription, 128, adapterDesc.Description, 128);
		if(error != 0) {
			return false;
		}

		// Release the display mode list.
		delete [] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if(ctx->vsync_enabled) {
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else {
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = hwnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if(fullscreen) {
			swapChainDesc.Windowed = false;
		}
		else {
			swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &ctx->swapChain, 
											   &ctx->device, NULL, &ctx->deviceContext);
		if(FAILED(result)) {
			return false;
		}

		// Get the pointer to the back buffer.
		result = ctx->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if(FAILED(result)) {
			return false;
		}

		// Create the render target view with the back buffer pointer.
		result = ctx->device->CreateRenderTargetView(backBufferPtr, NULL, &ctx->renderTargetView);
		if(FAILED(result)) {
			return false;
		}

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = ctx->device->CreateTexture2D(&depthBufferDesc, NULL, &ctx->depthStencilBuffer);
		if(FAILED(result)) {
			return false;
		}

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = ctx->device->CreateDepthStencilState(&depthStencilDesc, &ctx->depthStencilState);
		if(FAILED(result)) {
			return false;
		}

		// Set the depth stencil state.
		ctx->deviceContext->OMSetDepthStencilState(ctx->depthStencilState, 1);

		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = ctx->device->CreateDepthStencilView(ctx->depthStencilBuffer, &depthStencilViewDesc, &ctx->depthStencilView);
		if(FAILED(result)) {
			return false;
		}

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		ctx->deviceContext->OMSetRenderTargets(1, &ctx->renderTargetView, ctx->depthStencilView);

		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = ctx->device->CreateRasterizerState(&rasterDesc, &ctx->rasterState);
		if(FAILED(result)) {
			return false;
		}

		// Now set the rasterizer state.
		ctx->deviceContext->RSSetState(ctx->rasterState);
	
		// Setup the viewport for rendering.
		viewport.Width = (float)screenWidth;
		viewport.Height = (float)screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		ctx->deviceContext->RSSetViewports(1, &viewport);

		// Setup the projection matrix.
		//fieldOfView = (float)D3DX_PI / 4.0f;
		//screenAspect = (float)screenWidth / (float)screenHeight;

		// Create the projection matrix for 3D rendering.
		//D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

		// Initialize the world matrix to the identity matrix.
		//D3DXMatrixIdentity(&m_worldMatrix);

		// Create an orthographic projection matrix for 2D rendering.
		//D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

		// Clear the second depth stencil state before setting the parameters.
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
		// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the state using the device.
		result = ctx->device->CreateDepthStencilState(&depthDisabledStencilDesc, &ctx->depthDisabledStencilState);
		if(FAILED(result)) {
			return false;
		}
		ctx->camera.CreateProjectionMatrix(screenWidth, screenHeight, D3DX_PI / 3.0f, screenWidth/screenHeight, 0.1f, 1000.0f);
		ctx->camera.setPosition(0.0f,0.0f,-10.0f);
		ctx->camera.Update();

		assets::initialize();

		return true;
	}

}