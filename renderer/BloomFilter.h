#pragma once

class BloomFilter {

public:
	BloomFilter();
	~BloomFilter();
	void begin();
	void end();
	void render();
private:
	int _rt1;
};