#pragma once
#include <d3dx9core.h>

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