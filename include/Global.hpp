#ifndef MERLIN_INCLUDE_GLOBAL_HPP_
#define MERLIN_INCLUDE_GLOBAL_HPP_
#pragma once

#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

class Connection;
using ConnectionPtrT = std::shared_ptr<Connection>;

namespace Lancelot {
enum Side : std::uint8_t;
enum Exchange : std::uint8_t;

using ThreadPointerT = std::unique_ptr<std::jthread>;
using ThreadGroupT   = std::vector<ThreadPointerT>;
namespace API {
    enum OrderRequest : std::uint8_t;
    class StockPacket;
    class Strategy;

    using StockPacketPtrT = std::shared_ptr<StockPacket>;
    using StrategyPtrT    = std::shared_ptr<Strategy>;
    using StrategyParamT  = std::unordered_map<std::string, std::string>;
}// namespace API
}// namespace Lancelot

namespace Global {

class AddressDemangler {
  public:
    explicit AddressDemangler(uint32_t address_);
    explicit AddressDemangler(uint32_t connectionId_, uint32_t strategyId_);

    [[nodiscard]] uint32_t getConnectionId() const;
    [[nodiscard]] uint32_t getStrategyId() const;
    [[nodiscard]] uint32_t getAddress() const;

  private:
    uint32_t _connectionId;
    uint32_t _strategyId;
    uint32_t _address;
};

std::string TimeStampToHReadable(time_t time_);

void NewConnectionRequested(uint32_t loginId_, const ConnectionPtrT& connection_);

void ConnectionClosed(uint32_t loginId_);

ConnectionPtrT GetConnection(uint32_t loginId_);

void EventReceiver(uint32_t token_);

void AdaptorLoader(Lancelot::ThreadGroupT& threadGroup_, std::string_view dll_, Lancelot::Exchange exchange_);

auto StrategyLoader(std::string_view name_, uint32_t address_, const Lancelot::API::StrategyParamT& param_) -> bool;

auto StrategyParamUpdate(uint32_t address_, const Lancelot::API::StrategyParamT& param_) -> bool;

auto StrategyStopRequest(uint32_t address_) -> bool;

void StrategyStopCompleted(uint32_t address_);

auto GetStrategyStatus(uint32_t strategy_) -> std::string;

auto Serialize(const Lancelot::API::StockPacketPtrT& stockPacket_) -> std::string;

auto RegisterStockPacket(uint32_t token_, Lancelot::Side side_, const std::string& client_, const std::string& algo_, int ioc_, const Lancelot::API::StrategyPtrT& strategy_)
    -> Lancelot::API::StockPacketPtrT;

void ArthurMessageManagerThread(const std::stop_token& stopToken_);
}// namespace Global

#endif// MERLIN_INCLUDE_GLOBAL_HPP_