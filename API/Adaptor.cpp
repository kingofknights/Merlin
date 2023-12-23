
#include <Gawain.hpp>
#include <Lancelot.hpp>

#include "../include/Structure.hpp"

namespace Lancelot::API {
void Adaptor::OrderResponse(const Lancelot::API::StockPacketPtrT&, OrderStatus) {
}

void Adaptor::OnDisconnection(Lancelot::Exchange exchange_) { LOG(ERROR, "Exchange got disconnected : {}", Lancelot::toString(exchange_)) }

void Adaptor::OnConnection(Lancelot::Exchange exchange_) { LOG(INFO, "Exchange connected : {}", Lancelot::toString(exchange_)) }

}// namespace Lancelot::API