#include "BloomFilter.h"
#include "gfx.h"

BloomFilter::BloomFilter() {
	_rt1 = gfx::createRenderTarget();
}

BloomFilter::~BloomFilter() {
}

void BloomFilter::begin() {
	gfx::setRenderTarget(_rt1);
	gfx::clearRenderTarget(_rt1,Color(0,0,0));
}

void BloomFilter::end() {
	// draw RT1 using bloom shader to RT2
	// draw RT2 using vblur to RT3
	// draw RT3 using hblur to RT4
	gfx::restoreBackBuffer();	
	// draw RT1
	// draw RT4 using alpha blend
}

void BloomFilter::render() {
	gfx::drawRenderTarget(_rt1);
}