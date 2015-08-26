#include "stringutils.h"
#include <iostream>
#include <sstream>
#include <locale>
#include <ctime>

namespace string {

	int split(const std::string& str,const char delimiter,std::vector<std::string>& tokens) {	
		tokens.clear();
		std::string word;
		std::stringstream stream(str);
		int cnt = 0;
		while( getline(stream, word, delimiter) ) {
			tokens.push_back(word);
			++cnt;
		}
		return cnt;
	}

	IdString murmur_hash(const char* text) {
		return murmur_hash(text,strlen(text),0);
	}
	//-----------------------------------------------------------------------------
	// MurmurHash2, by Austin Appleby
	//-----------------------------------------------------------------------------
	IdString murmur_hash(const void * key, int len, unsigned int seed) {
		// 'm' and 'r' are mixing constants generated offline.
		// They're not really 'magic', they just happen to work well.

		const unsigned int m = 0x5bd1e995;
		const int r = 24;

		// Initialize the hash to a 'random' value

		IdString h = seed ^ len;

		// Mix 4 bytes at a time into the hash

		const unsigned char * data = (const unsigned char *)key;

		while(len >= 4) {
			unsigned int k = *(unsigned int *)data;

			k *= m; 
			k ^= k >> r; 
			k *= m; 

			h *= m; 
			h ^= k;

			data += 4;
			len -= 4;
		}

		// Handle the last few bytes of the input array

		switch(len) {
			case 3: h ^= data[2] << 16;
			case 2: h ^= data[1] << 8;
			case 1: h ^= data[0];
			h *= m;
		};

		// Do a few final mixes of the hash to ensure the last few
		// bytes are well-incorporated.

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}

}