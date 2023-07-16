#include "../include/Structure.hpp"
#include "LancelotAPI.hpp"

namespace Lancelot::API {
	void Adaptor::OrderResponse(const Lancelot::API::StockPacketPtrT& stockPacket_, OrderStatus status_) {
		stockPacket_->setPreviousStatus(stockPacket_->getCurrentStatus());
		stockPacket_->setCurrentStatus(status_);

		auto uniqueId = stockPacket_->getUniqueClassIdentity();
		if (stockPacket_->getStrategyPtr()) {
			stockPacket_->getStrategyPtr()->orderEvent(uniqueId);
		} else {
			switch (status_) {
				case OrderStatus_NEW: {
					break;
				}
				case OrderStatus_FILLED:
				case OrderStatus_CANCELLED: {
					break;
				}
				default: break;
			}
		}
	}

	void Adaptor::OnDisconnection(Lancelot::Exchange exchange_) { LOG(ERROR, "Exchange got disconnected : {}", Lancelot::toString(exchange_)) }

	void Adaptor::OnConnection(Lancelot::Exchange exchange_) { LOG(INFO, "Exchange connected : {}", Lancelot::toString(exchange_)) }

}  // namespace Lancelot::API