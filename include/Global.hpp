#pragma once

#include <memory>
#include <string>

class Connection;
class Strategy;
struct OrderPacketT;
enum SideType;
enum OrderRequest;

using OrderPacketPtrT = std::shared_ptr<OrderPacketT>;
using StrategyPtrT	  = std::shared_ptr<Strategy>;
namespace Global {

void NewConnectionRequested(uint64_t loginID_, const Connection *connection_);

void ConnectionClosed(uint64_t loginID_);

void UpdateEvent(int token_);

std::string GetStrategyStatus(int pf_);

OrderPacketPtrT RegisterOrderPacket(int token_, SideType side_, const std::string &client_, const std::string &algo_, int ioc_, const StrategyPtrT &strategy_);

void PlaceOrder(const OrderPacketPtrT &orderPacket_, int price_, int quantity_, OrderRequest request_);
}  // namespace Global
