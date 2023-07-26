#ifndef MERLIN_INCLUDE_STRUCTURE_HPP_
#define MERLIN_INCLUDE_STRUCTURE_HPP_
#pragma once

#include <Lancelot.hpp>
#include <array>
#include <boost/dll/shared_library.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <vector>
class SocketServer;

using SharedLibPtrT = std::unique_ptr<boost::dll::shared_library>;

namespace Lancelot::API {
	class Adaptor;
	using AdaptorPtrT = std::shared_ptr<Adaptor>;
}  // namespace Lancelot::API
#pragma pack(push, 1)

using AdaptorConnectionT = struct AdaptorConnectionT {
	Lancelot::API::AdaptorPtrT _adaptorPtr;
	SharedLibPtrT			   _sharedLibPtr;
};

#pragma pack(pop)

namespace Lancelot::API {
	class Strategy;
}
using AdaptorContainerT	 = std::array<AdaptorConnectionT, Lancelot::Exchange_END>;
using ThreadPointerT	 = std::unique_ptr<std::jthread>;
using ThreadGroupT		 = std::vector<ThreadPointerT>;
using SocketServerPtrT	 = std::shared_ptr<SocketServer>;
using StrategyPtrT		 = std::shared_ptr<Lancelot::API::Strategy>;
using StrategyContainerT = std::unordered_map<int, StrategyPtrT>;
using EventContainerT	 = std::unordered_map<int, StrategyContainerT>;

#endif	// MERLIN_INCLUDE_STRUCTURE_HPP_