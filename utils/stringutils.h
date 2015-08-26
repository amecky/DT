#pragma once
#include "..\stdafx.h"
#include <vector>

namespace string {

	int split(const std::string& str,const char delimiter,std::vector<std::string>& tokens);

	IdString murmur_hash(const char* text);

	IdString murmur_hash(const void * key, int len, unsigned int seed);

}