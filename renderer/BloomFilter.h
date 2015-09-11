#pragma once
#include "gfx.h"
#include "ScreenQuad.h"

struct BloomSettings {

	float intensity;
	float originalIntensity;
	float saturation;
	float originalSaturation;
};

class BloomFilter {

public:
	BloomFilter();
	~BloomFilter();
	void begin();
	void end();
	void render();
private:
	int _rt1;
	int _rt2;
	int _rt3;
	int _blCB;
	BasicShader* _bloomShader;
	BasicShader* _combineShader;
	BasicShader* _hBlurShader;
	BasicShader* _vBlurShader;
	ScreenQuad _quad;
};