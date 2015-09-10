#include "BloomFilter.h"
#include "gfx.h"
#include "RenderTarget.h"

BloomFilter::BloomFilter() {
	_rt1 = gfx::createRenderTarget();
	_rt2 = gfx::createRenderTarget();
	_bloomShader = gfx::createBasicShader("bloom.vs", "bloom.ps");
	_combineShader = gfx::createBasicShader("bloom_combine.vs", "bloom_combine.ps");
	_quad.initialize();
	_blCB = gfx::createConstantBuffer(sizeof(BloomSettings));
}

BloomFilter::~BloomFilter() {
}

void BloomFilter::begin() {
	gfx::setRenderTarget(_rt1);
	gfx::clearRenderTarget(_rt1,Color(0,0,0));
}

void BloomFilter::end() {
	gfx::restoreBackBuffer();
}

void BloomFilter::render() {
	// draw RT1 using bloom shader to RT2
	// draw RT2 using vblur to RT3
	// draw RT3 using hblur to RT4
	ID3D11ShaderResourceView* srv1 = gfx::getRenderTargetSRV(_rt1);	
	gfx::setRenderTarget(_rt2);
	gfx::clearRenderTarget(_rt2, Color(0, 0, 0));
	_quad.render(_bloomShader, srv1);
	ID3D11ShaderResourceView* srv2 = gfx::getRenderTargetSRV(_rt2);
	// draw RT1 and RT4 using combine shader
	gfx::restoreBackBuffer();
	gfx::beginRendering(Color(0, 0, 0));

	BloomSettings settings;
	settings.intensity = 1.0f;
	settings.originalIntensity = 0.7f;
	settings.saturation = 0.8f;
	settings.originalSaturation = 1.0f;
	ConstantBuffer* cb = gfx::getConstantBuffer(_blCB);
	cb->setData(gfx::getDeviceContext(), settings);
	cb->setPSBuffer(gfx::getDeviceContext(), 0);
	_quad.render(_combineShader, srv1,srv2);
	//gfx::drawRenderTarget(_rt2);

}