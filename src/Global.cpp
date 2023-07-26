#include "../include/Global.hpp"

#include <Lancelot.hpp>
#include <LancelotAPI.hpp>
#include <nlohmann/json.hpp>

#include "../include/Connection.hpp"
#include "../include/Structure.hpp"

static constexpr char _globalSharedLibEntryFunctionName[] = "getObject";

using ContainerPtrContainerT = std::unordered_map<uint64_t, const Connection*>;

namespace MerlinShared {
	extern int					  _globalJsonResponseUniqueId;
	extern EventContainerT		  _globalEventContainer;
	extern AdaptorContainerT	  _globalAdaptorContainer;
	extern StrategyContainerT	  _globalStrategyContainer;
	static ContainerPtrContainerT _globalContainerPtrContainer;
}  // namespace MerlinShared

void Global::NewConnectionRequested(uint64_t loginID_, const Connection* connection_) { MerlinShared::_globalContainerPtrContainer.insert_or_assign(loginID_, connection_); }

void Global::ConnectionClosed(uint64_t loginID_) {
	auto iterator = MerlinShared::_globalContainerPtrContainer.find(loginID_);
	if (iterator != MerlinShared::_globalContainerPtrContainer.end()) {
		delete iterator->second;
		MerlinShared::_globalContainerPtrContainer.erase(iterator);
	}
}

std::string Global::GetStrategyStatus(int pf_) {
	nlohmann::json json;
	json[JSON_ID] = ++MerlinShared::_globalJsonResponseUniqueId;

	nlohmann::json params;
	params[JSON_PF_NUMBER] = pf_;

	json[JSON_PARAMS] = params;
	return json.dump();
}

void Global::EventReceiver(int token_) {
	const auto iterator = MerlinShared::_globalEventContainer.find(token_);
	if (iterator == MerlinShared::_globalEventContainer.cend()) return;

	auto& list = iterator->second;
	for (const auto& [_, strategy_] : list) {
		if (strategy_->activated()) {
			strategy_->marketEvent(token_);
		}
	}
}

void Global::AdaptorLoader(ThreadGroupT& threadGroup_, std::string_view dll_, Lancelot::Exchange exchange_) {
	AdaptorConnectionT connection;
	connection._sharedLibPtr = std::make_unique<boost::dll::shared_library>(dll_.data(), boost::dll::load_mode::rtld_lazy);
	LOG(INFO, "Adaptor request to open is loaded [{}], filename [{}]", connection._sharedLibPtr->is_loaded(), dll_)
	LOG(INFO, "Adaptor [{}] , looking for [{}] found [{}]", dll_, _globalSharedLibEntryFunctionName, connection._sharedLibPtr->has(_globalSharedLibEntryFunctionName))

	if (not connection._sharedLibPtr->is_loaded()) return;
	if (not connection._sharedLibPtr->has(_globalSharedLibEntryFunctionName)) return;

	auto getObject = connection._sharedLibPtr->get<Lancelot::API::AdaptorPtrT(ThreadGroupT&)>(_globalSharedLibEntryFunctionName);

	LOG(INFO, "Adaptor invoking the function [{}]", _globalSharedLibEntryFunctionName)

	connection._adaptorPtr								= std::invoke(getObject, threadGroup_);
	MerlinShared::_globalAdaptorContainer.at(exchange_) = std::move(connection);
}

bool Global::StrategyLoader(std::string_view name_, int strategy_, const Lancelot::API::StrategyParamT& param_) {
	if (not std::filesystem::exists(name_)) {
		LOG(WARNING, "Strategy file not found : {}", name_)
		return false;
	}

	auto dll = std::make_unique<boost::dll::shared_library>(name_.data(), boost::dll::load_mode::rtld_lazy);
	if (not dll->is_loaded()) {
		LOG(WARNING, "Strategy failed to load : {}", name_)
		return false;
	}

	if (not dll->has(_globalSharedLibEntryFunctionName)) {
		LOG(WARNING, "Strategy does not have request entry point : {} {}", name_, _globalSharedLibEntryFunctionName)
		return false;
	}

	auto getObject = dll->get<StrategyPtrT(int, Lancelot::API::StrategyParamT)>(_globalSharedLibEntryFunctionName);
	auto strategy  = std::invoke(getObject, strategy_, param_);
	auto success   = MerlinShared::_globalStrategyContainer.emplace(strategy_, strategy);

	return success.second;
}

bool Global::StrategyParamUpdate(int strategy_, const Lancelot::API::StrategyParamT& param_) {
	const auto iterator = MerlinShared::_globalStrategyContainer.find(strategy_);
	if (iterator != MerlinShared::_globalStrategyContainer.cend()) {
		iterator->second->paramEvent(param_);
		return true;
	}
	return false;
}

bool Global::StrategyStopRequest(int strategy_) {
	const auto iterator = MerlinShared::_globalStrategyContainer.find(strategy_);
	if (iterator != MerlinShared::_globalStrategyContainer.cend()) {
		iterator->second->stopEvent();
		return true;
	}
	return false;
}
Lancelot::API::StockPacketPtrT Global::RegisterStockPacket(int token_, Lancelot::Side side_, const std::string& client_, const std::string& algo_, int ioc_, const StrategyPtrT& strategy_) {
	Lancelot::API::StockPacketPtrT stockPacket = std::make_shared<Lancelot::API::StockPacket>();
	stockPacket->setResultSetPtr(Lancelot::ContractInfo::GetResultSet(token_));
	stockPacket->setStrategyPtr(strategy_);
	stockPacket->setAdaptorPtr(MerlinShared::_globalAdaptorContainer[Lancelot::ContractInfo::GetExchange(token_)]._adaptorPtr);

	stockPacket->setIoc(ioc_);
	stockPacket->setPrice(0);
	stockPacket->setQuantity(0);
	stockPacket->setTotalQuantity(0);
	stockPacket->setLastPrice(0);
	stockPacket->setLastQuantity(0);
	stockPacket->setOrderNumber(0);
	stockPacket->setSide(side_);
	stockPacket->setClientCode(client_);
	stockPacket->setAlgoCode(algo_);
	stockPacket->setCurrentStatus(Lancelot::API::OrderStatus_NONE);
	stockPacket->setPreviousStatus(Lancelot::API::OrderStatus_NONE);

	return stockPacket;
}
