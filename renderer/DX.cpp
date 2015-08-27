#include "..\StdAfx.h"
#include "DX.h"
#include "..\utils\Log.h"
#include "VIBuffer.h"
#include "Shader.h"

// https://googledrive.com/host/0B0ND0J8HHfaXUTFFbjRPdWdkT28/DirectX_Template.zip

DX::DX(void) {
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
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &_swapchain,
                                  &_device,
                                  NULL,
                                  &_context);

	// get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    _device->CreateRenderTargetView(pBackBuffer, NULL, &_backbuffer);
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
    D3DX11CompileFromFile("shader.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile("shader.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

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
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 18, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    HR(_device->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &_layout));
    _context->IASetInputLayout(_layout);
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

	//int addBS = renderer::createBlendState("alpha_blend", ds::BL_ONE, ds::BL_ONE, true);
}

void DX::begin(const D3DXCOLOR& clearColor) {
	_context->ClearRenderTargetView(_backbuffer, clearColor);
	/*
	// clear the window to a deep blue
    //_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	_device->Clear(0, NULL, D3DCLEAR_TARGET, clearColor, 1.0f, 0);
	//_device->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    _device->BeginScene();    
	_camera.Update();
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
	_swapchain->Present(0, 0);
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
	_layout->Release();
    _VS->Release();
    _PS->Release();
	_swapchain->Release();
	_backbuffer->Release();
    _device->Release();
    _context->Release();
	//_d3d->Release();
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
	int cnt = 0;
	VDElement* tmp = elements;
	while ( tmp->type != VT_END ) {
		++cnt;
		++tmp;
	}
	LOG << "number of elements: " << cnt;
	D3DVERTEXELEMENT9* vdElements = new D3DVERTEXELEMENT9[cnt+1];
	int offset = 0;
	int counter[10];
	for (int i = 0; i < 10; ++i) {
		counter[i] = 0;
	}
	tmp = elements;
	int i = 0;
	VertexDeclaration decl;
	while ( tmp->type != VT_END ) {
		vdElements[i].Stream = 0;
		vdElements[i].Offset = offset;			
		switch ( tmp->type ) {
			case VT_FLOAT2 : 
				vdElements[i].Type = D3DDECLTYPE_FLOAT2;
				offset += 8;
				decl.size += 8;
				break;
			case VT_FLOAT3 : 
				vdElements[i].Type = D3DDECLTYPE_FLOAT3;
				offset += 12;
				decl.size += 12;
				break;
			case VT_FLOAT4 : 
				vdElements[i].Type = D3DDECLTYPE_FLOAT4;
				offset += 16;
				decl.size += 16;
				break;
			case VT_COLOR : 
				vdElements[i].Type = D3DDECLTYPE_D3DCOLOR;
				offset += 4;
				decl.size += 16;
				break;
		}
			
		vdElements[i].Method = D3DDECLMETHOD_DEFAULT;

		switch (tmp->usage ) {
			case VDU_POSITION : 
				vdElements[i].Usage = D3DDECLUSAGE_POSITION;
				vdElements[i].UsageIndex = counter[0];
				++counter[0];
				break;
			case VDU_POSITIONT : 
				vdElements[i].Usage = D3DDECLUSAGE_POSITIONT;
				vdElements[i].UsageIndex = counter[1];
				++counter[1];
				break;
			case VDU_COLOR : 
				vdElements[i].Usage = D3DDECLUSAGE_COLOR;
				vdElements[i].UsageIndex = counter[2];
				++counter[2];
				break;
			case VDU_TANGENT : 
				vdElements[i].Usage = D3DDECLUSAGE_TANGENT;
				vdElements[i].UsageIndex = counter[3];
				++counter[3];
				break;
			case VDU_BINORMAL : 
				vdElements[i].Usage = D3DDECLUSAGE_BINORMAL;
				vdElements[i].UsageIndex = counter[4];
				++counter[4];
				break;
			case VDU_NORMAL : 
				vdElements[i].Usage = D3DDECLUSAGE_NORMAL;
				vdElements[i].UsageIndex = counter[5];
				++counter[5];
				break;
			case VDU_TEXCOORD : 
				vdElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
				vdElements[i].UsageIndex = counter[6];
				++counter[6];
				break;
		}
		++i;
		++tmp;
	}
	// closing element
	vdElements[cnt].Stream = 0xff;
	vdElements[cnt].Offset = 0;
	vdElements[cnt].Type = D3DDECLTYPE_UNUSED;
	vdElements[cnt].Method = 0;
	vdElements[cnt].Usage = 0;
	vdElements[cnt].UsageIndex = 0;
//	_device->CreateVertexDeclaration(vdElements, &decl.declaration);
	_declarationMap[type] = decl;
	delete[] vdElements;
}

void DX::setVertexDeclaration(int type) {
	//_device->SetVertexDeclaration(_declarationMap[type].declaration);
}

int DX::createBlendState(int srcRGB, int srcAlpha, int dstRGB, int dstAlpha, bool alphaEnabled, bool separateAlpha) {
		return -1;
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
	return -1;
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
	_wvp = worldMatrix * _camera.GetViewMatrix() * _camera.GetProjectionMatrix();
}

void DX::selectVertexDeclaration(int id) {
	if ( _currentDeclaration != id ) {
		_currentDeclaration = id;
		setVertexDeclaration(id);
	}
}

void DX::prepareShader(Shader* shader, int textureID) {			
	
}

Shader* DX::getShader(int id) const {
	return _shaders[id];
}