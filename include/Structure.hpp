#ifndef MERLIN_INCLUDE_STRUCTURE_HPP_
#define MERLIN_INCLUDE_STRUCTURE_HPP_
#pragma once

#include <Lancelot/Enums.hpp>
#include <array>
#include <boost/dll/shared_library.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <variant>
#include <vector>

class SocketServer;

using SharedLibPtrT = std::unique_ptr<boost::dll::shared_library>;

namespace Lancelot::API {
class Strategy;
class Adaptor;
class StockPacket;

using AdaptorPtrT     = std::shared_ptr<Adaptor>;
using StockPacketPtrT = std::shared_ptr<StockPacket>;
}// namespace Lancelot::API
#pragma pack(push, 1)

using AdaptorConnectionT = struct AdaptorConnectionT {
    Lancelot::API::AdaptorPtrT _adaptorPtr;
    SharedLibPtrT              _sharedLibPtr;
};

#pragma pack(pop)

using AdaptorContainerT   = std::array<AdaptorConnectionT, Lancelot::Exchange_END>;
using ThreadPointerT      = std::unique_ptr<std::jthread>;
using ThreadGroupT        = std::vector<ThreadPointerT>;
using SocketServerPtrT    = std::shared_ptr<SocketServer>;
using StrategyPtrT        = std::shared_ptr<Lancelot::API::Strategy>;
using StrategyContainerT  = std::unordered_map<uint32_t, StrategyPtrT>;
using EventContainerT     = std::unordered_map<uint32_t, StrategyContainerT>;
using ArthurMessageTypeT  = std::variant<Lancelot::API::StockPacketPtrT>;
using ArthurMessageQueueT = boost::lockfree::spsc_queue<ArthurMessageTypeT, boost::lockfree::capacity<10000>>;

#endif// MERLIN_INCLUDE_STRUCTURE_HPP_