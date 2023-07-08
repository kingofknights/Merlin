//
// Created by admin on 11/11/21.
//

#include "../include/Compression.h"

#include <xcd/libxcd.h>

#include "../include/Enums.hpp"
#include "../include/Structure.hpp"

RequestInPackT Compression::Encrypt(std::string_view order_, uint64_t uid_, int type_) {
	RequestInPackT requestInPack;
	requestInPack._userIdentifier = uid_;
	requestInPack._totalSize	  = sizeof(RequestInPackT);
	requestInPack._type			  = type_;
	int			  CompressedDataLength;
	unsigned char CompressedData[512];
	xcdCompress(reinterpret_cast<const unsigned char*>(order_.data()), order_.length(), CompressedData, &CompressedDataLength);
	requestInPack._encryptLength = CompressedDataLength;
	memcpy(requestInPack._encryptMessage.data(), CompressedData, CompressedDataLength);
	return requestInPack;
}

int Compression::Decrypt(const unsigned char* data_, int size_, unsigned char* rawData_, int* rawLength_) { return xcdDecompress(data_, size_, rawData_, rawLength_); }
