#include "../include/Global.hpp"

#include <nlohmann/json.hpp>

#include "../API/Adaptor.hpp"
#include "../API/OrderUtility.hpp"
#include "../API/Strategy.hpp"
#include "../include/Connection.hpp"
#include "../include/Structure.hpp"

using ContainerPtrContainerT = std::unordered_map<uint64_t, const Connection*>;

namespace details {
	extern int					  _globalJsonResponseUniqueId;
	extern int					  _globalUniqueOrderPacketId;
	extern EventContainerT		  _globalEventContainer;
	extern AdaptorContainerT	  _globalAdaptorContainer;
	extern StrategyContainerT	  _globalStrategyContainer;
	static ContainerPtrContainerT _globalContainerPtrContainer;
}  // namespace details
void Global::NewConnectionRequested(uint64_t loginID_, const Connection* connection_) { details::_globalContainerPtrContainer.insert_or_assign(loginID_, connection_); }

void Global::ConnectionClosed(uint64_t loginID_) {
	auto iterator = details::_globalContainerPtrContainer.find(loginID_);
	if (iterator != details::_globalContainerPtrContainer.end()) {
		delete iterator->second;
		details::_globalContainerPtrContainer.erase(iterator);
	}
}

std::string Global::GetStrategyStatus(int pf_) {
	nlohmann::json json;
	json[JSON_ID] = ++details::_globalJsonResponseUniqueId;

	nlohmann::json params;
	params[JSON_PF_NUMBER] = pf_;

	json[JSON_PARAMS] = params;
	return json.dump();
}

void Global::EventReceiver(int token_) {
	const auto iterator = details::_globalEventContainer.find(token_);
	if (iterator == details::_globalEventContainer.cend()) return;

	auto& list = iterator->second;
	for (const auto& [_, strategy_] : list) {
		if (strategy_->activated()) {
			strategy_->marketEvent(token_);
		}
	}
}

void Global::AdaptorLoader(ThreadGroupT& threadGroup_, std::string_view dll_, Lancelot::ExchangeCode exchange_) {
	auto dll = std::make_unique<boost::dll::shared_library>(dll_.data(), boost::dll::load_mode::rtld_lazy);
	LOG(WARNING, "dll {} is loaded and has function {}", dll->is_loaded(), dll->has(ENTRY_FUNCTION_NAME))

	// auto getDriver				= dll->get<AdaptorPtrT(ThreadGroupT&)>(ENTRY_FUNCTION_NAME);
	// auto adaptor				= std::invoke(getDriver, threadGroup_);
	// _globalAdaptorContainer[exchange_] = {adaptor, std::move(dll)};
}

void Global::AlgorithmLoader(std::string_view dll_, int pf_, const StrategyParameterT& param_) {
	auto dll	   = std::make_unique<boost::dll::shared_library>(dll_.data(), boost::dll::load_mode::rtld_lazy);
	auto getDriver = dll->get<StrategyPtrT(int, StrategyParameterT)>(ENTRY_FUNCTION_NAME);
	auto strategy  = std::invoke(getDriver, pf_, param_);
	details::_globalStrategyContainer.emplace(pf_, strategy);
}

OrderPacketPtrT Global::RegisterOrderPacket(int token_, Lancelot::SideType side_, const std::string& client_, const std::string& algo_, int ioc_, const StrategyPtrT& strategy_) {
	OrderPacketPtrT orderPacket			 = std::make_shared<OrderPacketT>();
	orderPacket->_internal._resultSetPtr = Lancelot::ContractInfo::GetResultSet(token_);
	orderPacket->_internal._strategyPtr	 = strategy_;
	orderPacket->_internal._adaptorPtr	 = details::_globalAdaptorContainer[Lancelot::ContractInfo::GetExchange(token_)]._adaptorPtr;
	orderPacket->_internal._uniqueId	 = ++details::_globalUniqueOrderPacketId;

	OrderUtility::SetIoc(orderPacket, ioc_);
	OrderUtility::SetPrice(orderPacket, 0);
	OrderUtility::SetQuantity(orderPacket, 0);
	OrderUtility::SetTotalQuantity(orderPacket, 0);
	OrderUtility::SetLastPrice(orderPacket, 0);
	OrderUtility::SetLastQuantity(orderPacket, 0);
	OrderUtility::SetOrderNumber(orderPacket, 0);
	OrderUtility::SetSide(orderPacket, side_);
	OrderUtility::SetPF(orderPacket, 0);
	OrderUtility::SetClientCode(orderPacket, client_);
	OrderUtility::SetAlgoID(orderPacket, algo_);
	OrderUtility::SetCurrentOrderStatus(orderPacket, OrderStatus_NONE);
	OrderUtility::SetPreviousOrderStatus(orderPacket, OrderStatus_NONE);

	return orderPacket;
}

void Global::PlaceOrder(const OrderPacketPtrT& orderPacket_, int price_, int quantity_, Lancelot::OrderRequest request_) {
	if (not orderPacket_->_internal._adaptorPtr) return;

	auto orderStatus = OrderUtility::GetCurrentOrderStatus(orderPacket_);
	OrderUtility::SetCurrentOrderStatus(orderPacket_, OrderStatus_PENDING);

	if (orderPacket_->_internal._adaptorPtr->execute(orderPacket_, price_, quantity_, request_)) {
		OrderUtility::SetPrice(orderPacket_, price_);
		OrderUtility::SetQuantity(orderPacket_, quantity_);
	} else {
		OrderUtility::SetCurrentOrderStatus(orderPacket_, orderStatus);
	}
}
