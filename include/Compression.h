//
// Created by admin on 11/11/21.
//

#pragma once
#include <string_view>

struct RequestInPackT;

class Compression {
public:
	static RequestInPackT CompressData(std::string_view data, uint64_t uid, int type_);

	static int DeCompressData(const unsigned char *data, int size, unsigned char *unCompressedData, int *unCompressedDataLength);
};
