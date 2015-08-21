#include "..\StdAfx.h"
#include "DX.h"
#include "..\utils\Log.h"
#include "VIBuffer.h"

DX::DX(void)
{
}


DX::~DX(void)
{
}

void DX::init(HWND hWnd) {
	_d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	D3DDISPLAYMODE d3ddm;
	_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	DWORD total;
	if (_d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, true, D3DMULTISAMPLE_NONMASKABLE, &total)) {
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
		d3dpp.MultiSampleQuality = total - 1;
		LOG << "multi sample is supported - quality level " << total - 1;
	}
	else {
		LOG << "NO multi sample is supported";
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
	}

    // create a device class using this information and information from the d3dpp stuct
    _d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &_device);

	_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	_device->SetRenderState(D3DRS_ZENABLE, TRUE); 
	_device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	_camera.setPosition(0.0f,0.0f,12.0f);
	_camera.Update();
	
	 D3DXMatrixIdentity(&_world);

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

	VDElement elements[] = {
		{VT_FLOAT3, VDU_POSITION } ,
		{VT_FLOAT2, VDU_TEXCOORD},
		{VT_FLOAT4, VDU_COLOR},
		{VT_END,VDU_END},
	};
	create(0,elements);

	_defaultBlendState = createBlendState(BL_SRC_ALPHA, BL_ONE_MINUS_SRC_ALPHA, true);
	//int addBS = renderer::createBlendState("alpha_blend", ds::BL_ONE, ds::BL_ONE, true);
}

void DX::begin() {
	_camera.tick();
	// clear the window to a deep blue
    _device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	//_device->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    _device->BeginScene();    
	_camera.Update();
	_device->SetTransform(D3DTS_VIEW,_camera.GetViewMatrix());
	_device->SetTransform(D3DTS_PROJECTION,_camera.GetProjectionMatrix());
	_device->SetTransform(D3DTS_WORLD,&_world);
	_currentBlendState = -1;
	setBlendState(_currentBlendState);
}

void DX::end() {
	_device->EndScene(); 
    _device->Present(NULL, NULL, NULL, NULL);
}

void DX::render() {

}

void DX::shutdown() {
	for ( size_t i = 0; i < _textureAssets.size(); ++i ) {
		_textureAssets[i].texture->Release();
	}
	BufferList::iterator it = _buffers.begin();
	while ( it != _buffers.end()) {
		(*it)->release();
		it = _buffers.erase(it);
	}
	_device->Release();
	_d3d->Release();
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
	int id = _textureAssets.size();
	TextureAsset ta;
	//ta.name = string::murmur_hash(name);
	int lw = D3DX_DEFAULT;
	int lh = D3DX_DEFAULT;
	D3DXIMAGE_INFO imageInfo;
	char fileName[256];
	sprintf(fileName, "content\\%s.png", name);
	LOGC("Renderer") << "Trying to load texture " << fileName;
	D3DXCreateTextureFromFileEx(_device, fileName, 0, 0, 1, 0,D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0x000000, &imageInfo, NULL, &ta.texture);
	ta.width = imageInfo.Width;
	ta.height = imageInfo.Height;
	LOGC("Renderer") << "ID: " << id << " Width: " << imageInfo.Width << " Height: " << imageInfo.Height << " mip levels " << imageInfo.MipLevels << " Format: " << imageInfo.Format;
	_textureAssets.push_back(ta);
	return id;		
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
	_device->CreateVertexDeclaration(vdElements, &decl.declaration);
	_declarationMap[type] = decl;
}

void DX::setVertexDeclaration(int type) {
	_device->SetVertexDeclaration(_declarationMap[type].declaration);
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
	_currentBlendState = id;
}

// ---------------------------------------------------------------------
// Sets the blend state without checking and update current blend state
// ---------------------------------------------------------------------
void DX::setBlendState(int id) {
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
}