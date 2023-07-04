#include "../include/Global.hpp"

#include <nlohmann/json.hpp>

#include "../API/Adaptor.hpp"
#include "../API/OrderUtility.hpp"
#include "../include/Connection.hpp"
#include "../include/Strategy.hpp"
#include "../include/Structure.hpp"

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

    OrderUtility::SetIoc(orderPacket, ioc_);
    OrderUtility::SetPrice(orderPacket, 0);
    OrderUtility::SetQuantity(orderPacket, 0);
    OrderUtility::SetTotalQuantity(orderPacket, 0);
    OrderUtility::SetLastPrice(orderPacket, 0);
    OrderUtility::SetLastQuantity(orderPacket, 0);
    OrderUtility::SetOrderNumber(orderPacket, 0);
    OrderUtility::SetSide(orderPacket, side_);

    orderPacket->OrderDetails.PF = 0;

    std::memset(orderPacket->OrderDetails.ClientCode, '\0', CLIENT_CODE_LENGTH);
    std::memset(orderPacket->OrderDetails.AlgoID, '\0', ALGO_ID_LENGTH);
    std::memcpy(orderPacket->OrderDetails.ClientCode, client_.data(), std::min(int(client_.length()), CLIENT_CODE_LENGTH));
    std::memcpy(orderPacket->OrderDetails.AlgoID, algo_.data(), std::min(int(algo_.length()), ALGO_ID_LENGTH));

    OrderUtility::SetCurrentOrderStatus(orderPacket, OrderStatus_NONE);
    OrderUtility::SetPreviousOrderStatus(orderPacket, OrderStatus_NONE);
    return orderPacket;
}

void Global::PlaceOrder(const OrderPacketPtrT& orderPacket_, int price_, int quantity_, OrderRequest request_) {
    if (orderPacket_->Internal.AdaptorPtr and orderPacket_->Internal.AdaptorPtr->execute(orderPacket_, price_, quantity_, request_)) {
        OrderUtility::SetPrice(orderPacket_, price_);
        OrderUtility::SetQuantity(orderPacket_, quantity_);
        OrderUtility::SetCurrentOrderStatus(orderPacket_, OrderStatus_PENDING);
    }
}
