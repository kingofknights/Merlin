#pragma once

#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

class Connection;

namespace Lancelot {
	enum Side : int;
	enum Exchange : int;

	namespace API {
		enum OrderRequest : int;
		class StockPacket;
		class Strategy;

		using StockPacketPtrT = std::shared_ptr<StockPacket>;
		using StrategyPtrT	  = std::shared_ptr<Strategy>;
		using StrategyParamT  = std::unordered_map<std::string, std::string>;
	}  // namespace API
}  // namespace Lancelot

using ThreadPointerT = std::unique_ptr<std::jthread>;
using ThreadGroupT	 = std::vector<ThreadPointerT>;

namespace Global {

	void NewConnectionRequested(uint64_t loginID_, const Connection* connection_);

	void ConnectionClosed(uint64_t loginID_);

	void EventReceiver(int token_);

	void AdaptorLoader(ThreadGroupT& threadGroup_, std::string_view dll_, Lancelot::Exchange exchange_);

	void AlgorithmLoader(std::string_view dll_, int pf_, const Lancelot::API::StrategyParamT& param_);

	std::string GetStrategyStatus(int strategy_);

	Lancelot::API::StockPacketPtrT RegisterStockPacket(int token_, Lancelot::Side side_, const std::string& client_, const std::string& algo_, int ioc_, const Lancelot::API::StrategyPtrT& strategy_);

}  // namespace Global
