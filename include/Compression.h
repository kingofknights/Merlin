//
// Created by admin on 11/11/21.
//

#pragma once
#include <string_view>

struct RequestInPackT;

namespace Compression {
	RequestInPackT Encrypt(std::string_view order_, uint64_t uid_, int type_);

	int Decrypt(const unsigned char* data_, int size_, unsigned char* rawData_, int* rawLength_);
}  // namespace Compression
