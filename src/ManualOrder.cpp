//
// Created by kingofknights on 7/25/23.
//

#include "../include/ManualOrder.hpp"

#include <Lancelot.hpp>

#include "../include/Global.hpp"

ManualOrder::ManualOrder(int strategy_) : Strategy(strategy_) {}
void ManualOrder::paramEvent([[maybe_unused]] const Lancelot::API::StrategyParamT&) {}
void ManualOrder::marketEvent([[maybe_unused]] uint32_t token_) {}
void ManualOrder::orderEvent([[maybe_unused]] int uniqueId_) { LOG(ERROR, "{} {}", __PRETTY_FUNCTION__, uniqueId_) }
void ManualOrder::stopEvent() {}

int ManualOrder::newOrder(uint32_t token_, Lancelot::Side side_, const std::string& client_, const std::string& algo_, int ioc_) {
    auto packet = getStockPacket(token_, side_, client_, algo_, ioc_, false);
    _manualOrderContainer.emplace(packet->getUniqueClassIdentity(), packet);
    return packet->getUniqueClassIdentity();
}
void ManualOrder::placeOrder(int uniqueId_, int price_, int quantity_, Lancelot::API::OrderRequest orderRequest_) {
    const auto iterator = _manualOrderContainer.find(uniqueId_);
    if (iterator == _manualOrderContainer.cend()) {
        LOG(WARNING, "not manual order packet is found for id [{}] price [{}] quantity [{}] type [{}]", uniqueId_, price_, quantity_, Lancelot::API::toString(orderRequest_));
        return;
    }

    iterator->second->execute(price_, quantity_, orderRequest_);
}
