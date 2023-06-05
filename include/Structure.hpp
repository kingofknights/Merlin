#pragma once

#include <array>

#pragma pack(push, 1)

using RequestInPackT = struct RequestInPackT {
	short				  TotalSize;
	int					  Type;
	uint64_t			  UserIdentifier;
	int					  CompressedMsgLen;
	std::array<char, 512> Message;
};

#pragma pack(pop)
