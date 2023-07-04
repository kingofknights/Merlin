#include "Adaptor.hpp"

#include "../API/OrderUtility.hpp"
#include "../include/Strategy.hpp"
#include "../include/Structure.hpp"

extern GlobalManualOrderPacketT GlobalManualOrderPacket;
Adaptor::Adaptor(boost::asio::io_context &ioContext_, ThreadGroupT &threadContainer_) {}

void Adaptor::OrderResponse(const OrderPacketPtrT &order_, OrderStatus status_) {
    order_->PreviousStatus = order_->CurrentStatus;
    order_->CurrentStatus  = status_;

    auto uniqueId = OrderUtility::GetUniqueID(order_);
    if (order_->Internal.StrategyPtr) {
        order_->Internal.StrategyPtr->orderEvent(uniqueId);
    } else {
        switch (status_) {
            case OrderStatus_NEW: {
                GlobalManualOrderPacket.emplace(uniqueId, order_);
                break;
            }
            case OrderStatus_FILLED:
            case OrderStatus_CANCELLED: {
                GlobalManualOrderPacket.erase(uniqueId);
                break;
            }
            default: break;
        }
    }
}

void Adaptor::OnDisconnection(Exchange exchange_) {
    LOG(ERROR, "Exchange got disconnected : {}", exchange_)
}

void Adaptor::OnConnection(Exchange exchange_) {
    LOG(INFO, "Exchange connected : {}", exchange_)
}
