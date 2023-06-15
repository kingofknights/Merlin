#include "../include/Global.hpp"

#include <Lancelot/ContractInfo.hpp>
#include <nlohmann/json.hpp>

#include "../include/Connection.hpp"
#include "../include/Strategy.hpp"
#include "../include/Structure.hpp"
#include "../API/Adaptor.hpp"

using ContainerPtrContainerT = std::unordered_map<uint64_t, const Connection*>;

static ContainerPtrContainerT ContainerPtrContainer;
extern int					  id;
extern int					  UniqueOrderPacketId;
extern EventContainerT		  EventContainer;
extern AdaptorContainerT	  AdaptorContainer;

void Global::NewConnectionRequested(uint64_t loginID_, const Connection* connection_) {
	ContainerPtrContainer.insert_or_assign(loginID_, connection_);
}

void Global::ConnectionClosed(uint64_t loginID_) {
	auto iterator = ContainerPtrContainer.find(loginID_);
	if (iterator != ContainerPtrContainer.end()) {
		if (iterator->second) {
			delete iterator->second;
		}
		ContainerPtrContainer.erase(iterator);
	}
}

std::string Global::GetStrategyStatus(int pf_) {
	nlohmann::json json;
	json[JSON_ID] = ++id;

	nlohmann::json params;
	params[JSON_PF_NUMBER] = pf_;

	json[JSON_PARAMS] = params;
	return json.dump();
}

void Global::UpdateEvent(int token_) {
	auto iterator = EventContainer.find(token_);
	if (iterator == EventContainer.end()) return;

	auto& list = iterator->second;
	for (const auto& [_, strategy_] : list) {
		if (strategy_->activated()) {
			strategy_->marketEvent(token_);
		}
	}
}

OrderPacketPtrT Global::RegisterOrderPacket(int token_, SideType side_, const std::string& client_, const std::string& algo_, int ioc_, const StrategyPtrT& strategy_) {
	OrderPacketPtrT orderPacket		   = std::make_shared<OrderPacketT>();
	orderPacket->Internal.ResultSetPtr = Lancelot::ContractInfo::GetResultSet(token_);
	orderPacket->Internal.StrategyPtr  = strategy_;
	orderPacket->Internal.AdaptorPtr   = AdaptorContainer[Lancelot::ContractInfo::GetExchange(token_)].AdaptorPtr;
	orderPacket->Internal.UniqueID	   = ++UniqueOrderPacketId;

	orderPacket->OrderDetails.Ioc		  = ioc_;
	orderPacket->OrderDetails.PF		  = 0;
	orderPacket->OrderDetails.Price		  = 0;
	orderPacket->OrderDetails.Quantity	  = 0;
	orderPacket->OrderDetails.OrderNumber = 0;
	orderPacket->OrderDetails.Side		  = side_;
	std::memset(orderPacket->OrderDetails.ClientCode, '\0', CLIENT_CODE_LENGTH);
	std::memset(orderPacket->OrderDetails.AlgoID, '\0', ALGO_ID_LENGTH);
	std::memcpy(orderPacket->OrderDetails.ClientCode, client_.data(), std::min(int(client_.length()), CLIENT_CODE_LENGTH));
	std::memcpy(orderPacket->OrderDetails.AlgoID, algo_.data(), std::min(int(algo_.length()), ALGO_ID_LENGTH));

	orderPacket->CurrentStatus	= OrderStatus_NONE;
	orderPacket->PreviousStatus = OrderStatus_NONE;

	return orderPacket;
}

void Global::PlaceOrder(const OrderPacketPtrT& orderPacket_, int price_, int quantity_, OrderRequest request_) {
	orderPacket_->OrderDetails.Price	= price_;
	orderPacket_->OrderDetails.Quantity = quantity_;
	orderPacket_->Request				= request_;

	if (orderPacket_->Internal.AdaptorPtr and orderPacket_->Internal.AdaptorPtr->place(orderPacket_, request_)) {
		orderPacket_->CurrentStatus =  OrderStatus_PENDING;
	}
}
