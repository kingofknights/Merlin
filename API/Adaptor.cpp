#include "Adaptor.hpp"

#include "../include/Strategy.hpp"
#include "../include/Structure.hpp"

Adaptor::Adaptor(boost::asio::io_context &ioContext_, ThreadGroupT &threadContainer_) {}

void Adaptor::OrderResponse(const OrderPacketPtrT &order_, OrderStatus status_) {
	order_->PreviousStatus = order_->CurrentStatus;
	order_->CurrentStatus  = status_;

	if (order_->Internal.StrategyPtr) {
	}
}

void Adaptor::OnDisconnection(Exchange exchange_) {}

void Adaptor::OnConnection(Exchange exchange_) {}
