#include "Adaptor.hpp"

#include "../API/OrderUtility.hpp"
#include "../API/Strategy.hpp"
#include "../include/Structure.hpp"

extern GlobalManualOrderPacketT GlobalManualOrderPacket;

void Adaptor::OrderResponse(const OrderPacketPtrT& order_, OrderStatus status_) {
	order_->_previousStatus = order_->_currentStatus;
	order_->_currentStatus	= status_;

	auto uniqueId = OrderUtility::GetUniqueID(order_);
	if (order_->_internal._strategyPtr) {
		order_->_internal._strategyPtr->orderEvent(uniqueId);
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

void Adaptor::OnDisconnection(Lancelot::ExchangeCode exchange_) { LOG(ERROR, "Exchange got disconnected : {}", static_cast<int>(exchange_)) }

void Adaptor::OnConnection(Lancelot::ExchangeCode exchange_) { LOG(INFO, "Exchange connected : {}", static_cast<int>(exchange_)) }
