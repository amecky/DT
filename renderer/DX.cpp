#include "..\StdAfx.h"
#include "DX.h"
#include "..\utils\Log.h"
#include "VIBuffer.h"
#include "Shader.h"
#include <assert.h>

// https://googledrive.com/host/0B0ND0J8HHfaXUTFFbjRPdWdkT28/DirectX_Template.zip



DX::DX() {
	_swapchain = 0;
	_device = 0;
	_context = 0;
	_renderTargetView = 0;
	_depthStencilBuffer = 0;
	_depthStencilState = 0;
	_depthStencilView = 0;
	_rasterState = 0;
	_depthDisabledStencilState = 0;
}


DX::~DX(void) {
}

void DX::init(HWND hWnd,int sizeX,int sizeY) {

	DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode

    // create a device, device context and swap chain using the information in the scd struct
    HR(D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
								  D3D11_CREATE_DEVICE_DEBUG,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &_swapchain,
                                  &_device,
                                  NULL,
                                  &_context));

	// get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    HR(_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

    // use the back buffer address to create the render target
    HR(_device->CreateRenderTargetView(pBackBuffer, NULL, &_backbuffer));
    pBackBuffer->Release();

    // set the render target as the back buffer
    _context->OMSetRenderTargets(1, &_backbuffer, NULL);

    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = sizeX;
    viewport.Height = sizeY;

    _context->RSSetViewports(1, &viewport);


	// load and compile the two shaders
    ID3D10Blob *VS, *PS;
    D3DX11CompileFromFile("vertex_shader.hlsl", 0, 0, "SimpleVertexShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile("pixel_shader.hlsl", 0, 0, "SimplePixelShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

    // encapsulate both shaders into shader objects
    HR(_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &_VS));
    HR(_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &_PS));

    // set the shader objects
    _context->VSSetShader(_VS, 0, 0);
    _context->PSSetShader(_PS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    HRESULT hr = _device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &_layout);
	assert(_layout != 0);
    

	// Setup rasterizer state.
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state object.
	HR(_device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState));


	// Create the constant buffers for the variables defined in the vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HR(_device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Appliation]));
	HR(_device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Frame]));
	HR(_device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Object]));
	
	/*
	_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	_device->SetRenderState(D3DRS_ZENABLE, TRUE); 
	_device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	*/
	_camera.setPosition(0.0f,0.0f,6.0f);
	_camera.Update();
	
	 D3DXMatrixIdentity(&_world);
	 /*
	 // create white texture as default
	 int id = _textureAssets.size();
	TextureAsset ta;
	//ta.name = string::murmur_hash(name);
	int lw = D3DX_DEFAULT;
	int lh = D3DX_DEFAULT;
	D3DXCreateTexture(_device, 256, 256, 0,0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ta.texture);
	ta.width = 256;
	ta.height = 256;
	LOGC("Renderer") << "ID: " << id;
	
	 D3DLOCKED_RECT area;

	ta.texture->LockRect(0,&area,0,D3DLOCK_DISCARD);
	DWORD *bytePointer=(DWORD*)area.pBits;

    for(DWORD i=0; i< 256; i++)  {
        for(DWORD j=0; j< 256; j++) {
            DWORD index = i * area.Pitch / 4 + j;
            bytePointer[index] = D3DCOLOR_XRGB(255,0,0); //make it red
        }
    }
	ta.texture->UnlockRect(0);
	_textureAssets.push_back(ta);
	*/
	VDElement elements[] = {
		{VT_FLOAT3, VDU_POSITION } ,
		{VT_FLOAT2, VDU_TEXCOORD},
		{VT_FLOAT4, VDU_COLOR},
		{VT_END,VDU_END},
	};
	create(0,elements);

	_defaultBlendState = createBlendState(BL_SRC_ALPHA, BL_ONE_MINUS_SRC_ALPHA, true);
	//int addBS = renderer::createBlendState("alpha_blend", ds::BL_ONE, ds::BL_ONE, true);
	_context->UpdateSubresource(_constantBuffers[CB_Appliation], 0, nullptr, &_camera.GetProjectionMatrix(), 0, 0);
}

void DX::begin(const D3DXCOLOR& clearColor) {
	_camera.Update();

	// Clear the back buffer.
	_context->ClearRenderTargetView(_renderTargetView, clearColor);

	// Clear the depth buffer.
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	/*

	_context->ClearRenderTargetView(_backbuffer, clearColor);
	*/
	_context->IASetInputLayout(_layout);

	_context->VSSetShader(_VS, nullptr, 0);
	
	_context->UpdateSubresource(_constantBuffers[CB_Frame], 0, nullptr, &_camera.GetViewMatrix(), 0, 0);
	_context->VSSetConstantBuffers(0, 3, _constantBuffers);

	_context->RSSetState(_rasterState);
	//_context->RSSetViewports(1, &_viewport);

	_context->PSSetShader(_PS, nullptr, 0);
	/*
	_context->OMSetRenderTargets(1, &_backbuffer, NULL);
	*/
	/*
	// clear the window to a deep blue
    //_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	_device->Clear(0, NULL, D3DCLEAR_TARGET, clearColor, 1.0f, 0);
	//_device->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    _device->BeginScene();    
	
	_device->SetTransform(D3DTS_VIEW,&_camera.GetViewMatrix());
	_device->SetTransform(D3DTS_PROJECTION,&_camera.GetProjectionMatrix());
	_device->SetTransform(D3DTS_WORLD,&_world);
	_currentBlendState = -1;
	_currentShader = -1;
	_currentTexture = -1;
	_currentDeclaration = -1;
	setBlendState(_currentBlendState);
	*/
}

void DX::end() {
	//_device->EndScene(); 
    //_device->Present(NULL, NULL, NULL, NULL);
	//_swapchain->Present(0, 0);
	if (_vsync_enabled)	{
		// Lock to screen refresh rate.
		_swapchain->Present(1, 0);
	}
	else {
		// Present as fast as possible.
		_swapchain->Present(0, 0);
	}

}

void DX::render() {

}

void DX::shutdown() {
	LOGC("DX") << "shutdown";
	for ( size_t i = 0; i < _textureAssets.size(); ++i ) {
		_textureAssets[i].texture->Release();
	}
	BufferList::iterator it = _buffers.begin();
	while ( it != _buffers.end()) {
		(*it)->release();
		delete (*it);
		it = _buffers.erase(it);
	}
	/*
	_layout->Release();
    _VS->Release();
    _PS->Release();
	_swapchain->Release();
	_backbuffer->Release();
    _device->Release();
    _context->Release();
	*/
	if (_swapchain)	{
		_swapchain->SetFullscreenState(false, NULL);
	}

	SAFE_RELEASE(_depthDisabledStencilState);
	SAFE_RELEASE(_rasterState);
	SAFE_RELEASE(_depthStencilView);
	SAFE_RELEASE(_depthStencilState);
	SAFE_RELEASE(_depthStencilBuffer);
	SAFE_RELEASE(_renderTargetView);
	SAFE_RELEASE(_context);
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_swapchain);	

}

int DX::createBuffer(const BufferDescriptor& desc) {
	VIBuffer* buffer = new VIBuffer(this,1024,desc.vertexSize,desc.declarationID);
	int id = _buffers.size();
	_buffers.push_back(buffer);
	return id;
}

VIBuffer* DX::getBuffer(int idx) const {
	return _buffers[idx];
}

int DX::loadTexture(const char* name) {
	/*
	int id = _textureAssets.size();
	TextureAsset ta;
	//ta.name = string::murmur_hash(name);
	int lw = D3DX_DEFAULT;
	int lh = D3DX_DEFAULT;
	D3DXIMAGE_INFO imageInfo;
	char fileName[256];
	sprintf(fileName, "content\\%s.png", name);
	LOGC("Renderer") << "Trying to load texture " << fileName;
	//D3DXCreateTextureFromFileEx(_device, fileName, 0, 0, 1, 0,D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0x000000, &imageInfo, NULL, &ta.texture);
	ta.width = imageInfo.Width;
	ta.height = imageInfo.Height;
	LOGC("Renderer") << "ID: " << id << " Width: " << imageInfo.Width << " Height: " << imageInfo.Height << " mip levels " << imageInfo.MipLevels << " Format: " << imageInfo.Format;
	_textureAssets.push_back(ta);
	return id;		
	*/
	return -1;
}

void DX::create(int type,VDElement* elements) {
	
}

void DX::setVertexDeclaration(int type) {
	//_device->SetVertexDeclaration(_declarationMap[type].declaration);
}

int DX::createBlendState(const BlendState& state) {
	_blendStates.push_back(state);
	return _blendStates.size() - 1;
}

int DX::createBlendState(int srcRGB, int srcAlpha, int dstRGB, int dstAlpha, bool alphaEnabled, bool separateAlpha) {
		BlendState bs;
		bs.blendEnable = alphaEnabled;
		bs.srcFactorRGB = srcRGB;
		bs.srcFactorAlpha = srcAlpha;
		bs.blendModeRGB = BM_ADD;
		bs.dstFactorRGB = dstRGB;
		bs.dstFactorAlpha = dstAlpha;
		bs.blendModeAlpha = BM_ADD;
		bs.alphaFunc = ALPHA_GREATER;
		bs.alphaRef = 0;
		bs.separateAlpha = separateAlpha;
		bs.mask = 0x0f;
		bs.flag = 1;
		_blendStates.push_back(bs);
		return _blendStates.size() - 1;
}

int DX::createBlendState(int srcAlpha, int dstAlpha, bool alphaEnabled) {
	return createBlendState(srcAlpha, srcAlpha, dstAlpha, dstAlpha, alphaEnabled);
}

// -----------------------------------------------------------------
// Change current blend state if necessary
// -----------------------------------------------------------------
void DX::changeBlendState(int id) {
	/*
	const BlendState& newState = _blendStates[id];
	const BlendState& current = _blendStates[_currentBlendState];
	if (!newState.blendEnable){
		if (current.blendEnable){
			_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}
	}
	else {
		if (newState.blendEnable){
			if (!current.blendEnable){
				_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			}
			if (newState.srcFactorRGB != current.srcFactorRGB){
				_device->SetRenderState(D3DRS_SRCBLEND, newState.srcFactorRGB);
			}
			if (newState.dstFactorRGB != current.dstFactorRGB){
				_device->SetRenderState(D3DRS_DESTBLEND, newState.dstFactorRGB);
			}
			if (newState.blendModeRGB != current.blendModeRGB){
				_device->SetRenderState(D3DRS_BLENDOP, newState.blendModeRGB);
			}
			if (newState.srcFactorAlpha != current.srcFactorAlpha){
				_device->SetRenderState(D3DRS_SRCBLENDALPHA, newState.srcFactorAlpha);
			}
			if (newState.dstFactorAlpha != current.dstFactorAlpha){
				_device->SetRenderState(D3DRS_DESTBLENDALPHA, newState.dstFactorAlpha);
			}
			if (newState.blendModeAlpha != current.blendModeAlpha){
				_device->SetRenderState(D3DRS_BLENDOPALPHA, newState.blendModeAlpha);
			}
			if (newState.separateAlpha != current.separateAlpha){
				_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, newState.separateAlpha);
			}
			if (newState.alphaRef != current.alphaRef){
				_device->SetRenderState(D3DRS_ALPHAREF, (DWORD)newState.alphaRef);
			}
			if (newState.alphaFunc != current.alphaFunc){
				_device->SetRenderState(D3DRS_ALPHAFUNC, newState.alphaFunc);
			}
			if (newState.blendOp != current.blendOp){
				_device->SetRenderState(D3DRS_BLENDOP, newState.blendOp);
			}
		}
	}
	*/
	_currentBlendState = id;
}

// ---------------------------------------------------------------------
// Sets the blend state without checking and update current blend state
// ---------------------------------------------------------------------
void DX::setBlendState(int id) {
	/*
	if (id != _currentBlendState) {
		const BlendState& newState = _blendStates[id];
		if (!newState.blendEnable){
			_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}
		else {
			_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			_device->SetRenderState(D3DRS_SRCBLEND, newState.srcFactorRGB);
			_device->SetRenderState(D3DRS_DESTBLEND, newState.dstFactorRGB);
			_device->SetRenderState(D3DRS_BLENDOP, newState.blendModeRGB);
			_device->SetRenderState(D3DRS_SRCBLENDALPHA, newState.srcFactorAlpha);
			_device->SetRenderState(D3DRS_DESTBLENDALPHA, newState.dstFactorAlpha);
			_device->SetRenderState(D3DRS_BLENDOPALPHA, newState.blendModeAlpha);
			_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, newState.separateAlpha);
			_device->SetRenderState(D3DRS_ALPHAREF, (DWORD)newState.alphaRef);
			_device->SetRenderState(D3DRS_ALPHAFUNC, newState.alphaFunc);
			_device->SetRenderState(D3DRS_BLENDOP, newState.blendOp);
		}
		_currentBlendState = id;
	}
	*/
}

int DX::loadShader(const char* fileName,const char* techName) {
	Shader* s = new Shader(this);
	s->loadShader(fileName,techName);
	_shaders.push_back(s);
	return _shaders.size() - 1;
}

void DX::selectShader(int id) {
	if ( _currentShader != id ) {
		_currentShader = id;
	}
}

void DX::selectTexture(int id) {
	if ( _currentTexture != id ) {
		_currentTexture = id;
		//_device->SetTexture(0,getTexture(id));
	}
}

void DX::setWorldTransformation(const D3DXMATRIX& worldMatrix) {
	//_device->SetTransform(D3DTS_WORLD,&worldMatrix);
	_context->UpdateSubresource(_constantBuffers[CB_Object], 0, nullptr, &worldMatrix, 0, 0);
	_wvp = worldMatrix * _camera.GetViewMatrix() * _camera.GetProjectionMatrix();
}

void DX::selectVertexDeclaration(int id) {
	if ( _currentDeclaration != id ) {
		_currentDeclaration = id;
		setVertexDeclaration(id);
	}
}

void DX::prepareShader(Shader* shader, int textureID) {			
	shader->setValue("gWVP", _wvp,sizeof(D3DXMATRIX));
	shader->setValue("gWorld", _world,sizeof(D3DXMATRIX));		
	//shader->setVector3f("gCameraPos", renderContext->camera->getPosition());
	//shader->setVector3f("gCameraUp", renderContext->camera->getUpVector());
	if (textureID != -1) {
		shader->setTexture("gTex",textureID);
	}
	/*
	if (shader->contains("gWorldInverseTranspose")) {
		D3DXMATRIX worldInverseTranspose;
		D3DXMatrixInverse(&worldInverseTranspose, 0, &matrix::convert(renderContext->matWorld));
		D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);
		shader->setValue("gWorldInverseTranspose", &worldInverseTranspose, sizeof(D3DXMATRIX));
	}
	if (shader->contains("gWorldInverse")) {
		D3DXMATRIX worldInverse;
		D3DXMatrixInverse(&worldInverse, 0, &matrix::convert(renderContext->matWorld));
		shader->setValue("gWorldInverse", worldInverse,sizeof(D3DXMATRIX));
	}
	shader->setMatrix("gView", renderContext->matView);
	shader->setMatrix("gProjection", renderContext->matProjection);
	*/
	shader->commitChanges();
}

Shader* DX::getShader(int id) const {
	return _shaders[id];
}



bool DX::initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear) {
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
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;


	// Store the vsync setting.
	_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) {
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)	{
		if (displayModeList[i].Width == (unsigned int)screenWidth) {
			if (displayModeList[i].Height == (unsigned int)screenHeight) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
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
	if (_vsync_enabled) {
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
	if (fullscreen) {
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
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &_swapchain,
		&_device, NULL, &_context);
	if (FAILED(result))	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
	if (FAILED(result))	{
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
	result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if (FAILED(result))	{
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
	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(result))	{
		return false;
	}

	// Set the depth stencil state.
	_context->OMSetDepthStencilState(_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if (FAILED(result))	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	_context->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

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
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if (FAILED(result))	{
		return false;
	}

	// Now set the rasterizer state.
	_context->RSSetState(_rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	_context->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;
	/*
	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&m_worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	*/
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
	result = _device->CreateDepthStencilState(&depthDisabledStencilDesc, &_depthDisabledStencilState);
	if (FAILED(result))	{
		return false;
	}


	// Create the constant buffers for the variables defined in the vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	_camera.setPosition(0.0f, 0.0f, 6.0f);
	_camera.Update();

	HR(_device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Appliation]));
	HR(_device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Frame]));
	HR(_device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Object]));

	_context->UpdateSubresource(_constantBuffers[CB_Appliation], 0, nullptr, &_camera.GetProjectionMatrix(), 0, 0);

	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile("vertex_shader.hlsl", 0, 0, "SimpleVertexShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile("pixel_shader.hlsl", 0, 0, "SimplePixelShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	HR(_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &_VS));
	HR(_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &_PS));

	// set the shader objects
	_context->VSSetShader(_VS, 0, 0);
	_context->PSSetShader(_PS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr = _device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &_layout);
	assert(_layout != 0);

	return true;
}
