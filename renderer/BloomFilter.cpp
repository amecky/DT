#include "BloomFilter.h"
#include "gfx.h"
#include "RenderTarget.h"

BloomFilter::BloomFilter() {
	_rt1 = gfx::createRenderTarget();
	_rt2 = gfx::createRenderTarget();
	_rt3 = gfx::createRenderTarget();
	_bloomShader = gfx::createBasicShader("basic_ptc.vs", "bloom.ps");
	_combineShader = gfx::createBasicShader("basic_ptc.vs", "bloom_combine.ps");
	_hBlurShader = gfx::createBasicShader("hblur.vs","blur.ps");
	_vBlurShader = gfx::createBasicShader("vblur.vs","blur.ps");
	_quad.initialize();
	_blCB = gfx::createConstantBuffer(sizeof(BloomSettings));
}

BloomFilter::~BloomFilter() {
	delete _bloomShader;
	delete _combineShader;
	delete _hBlurShader;
	delete _vBlurShader;
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
	// RT1 = original source
	// RT2 = bloom
	ID3D11ShaderResourceView* srv1 = gfx::getRenderTargetSRV(_rt1);	
	gfx::setRenderTarget(_rt2);
	gfx::clearRenderTarget(_rt2, Color(0, 0, 0));
	_quad.render(_bloomShader, srv1);
	// RT3 - horizontal blur
	ID3D11ShaderResourceView* srv2 = gfx::getRenderTargetSRV(_rt2);
	gfx::setRenderTarget(_rt3);
	gfx::clearRenderTarget(_rt3, Color(0, 0, 0));
	_quad.render(_hBlurShader, srv2);
	// RT2 vertical blur
	ID3D11ShaderResourceView* srv3 = gfx::getRenderTargetSRV(_rt3);
	gfx::setRenderTarget(_rt2);
	gfx::clearRenderTarget(_rt2, Color(0, 0, 0));
	_quad.render(_vBlurShader, srv3);
	
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