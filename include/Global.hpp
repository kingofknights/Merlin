#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

class Connection;
class Strategy;
struct OrderPacketT;
enum SideType;
enum OrderRequest;
enum Exchange;

using OrderPacketPtrT	 = std::shared_ptr<OrderPacketT>;
using StrategyPtrT		 = std::shared_ptr<Strategy>;
using StrategyParameterT = std::unordered_map<std::string, std::string>;

using ThreadPointerT = std::unique_ptr<std::jthread>;
using ThreadGroupT	 = std::vector<ThreadPointerT>;

namespace Global {

void NewConnectionRequested(uint64_t loginID_, const Connection *connection_);

void ConnectionClosed(uint64_t loginID_);

void EventReceiver(int token_);

void AdaptorLoader(ThreadGroupT &threadGroup_, std::string_view dll_, Exchange exchange_);

void AlogrithmLoader(std::string_view dll_, int pf_, StrategyParameterT param_);

std::string GetStrategyStatus(int pf_);

OrderPacketPtrT RegisterOrderPacket(int token_, SideType side_, const std::string &client_, const std::string &algo_, int ioc_, const StrategyPtrT &strategy_);

void PlaceOrder(const OrderPacketPtrT &orderPacket_, int price_, int quantity_, OrderRequest request_);
}  // namespace Global
