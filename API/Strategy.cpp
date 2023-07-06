#include "Strategy.hpp"

#include "../include/Global.hpp"
#include "../include/Logger.hpp"
#include "../include/Structure.hpp"

extern EventContainerT	  EventContainer;
extern StrategyContainerT StrategyContainer;

Strategy::Strategy(int pf_, const StrategyParameterT &strategyParameter_) : _pf(pf_), _activated(false) {
    LOG(INFO, "New Strategy Requested {}", _pf)
    LOG(INFO, "Paramters {}", strategyParameter_)
}

void Strategy::terminate() {
    setActivated(false);
    StrategyContainer.erase(_pf);
    for (const auto &[_, container] : EventContainer) {
        EventContainer.erase(_pf);
    }
}

OrderPacketPtrT Strategy::getOrderPacket(int token_, SideType side_, const std::string &client_, const std::string &algo_, int ioc_) {
    return Global::RegisterOrderPacket(token_, side_, client_, algo_, ioc_, shared_from_this());
}

void Strategy::registerForData(int token_) {
    LOG(INFO, "{} PF={} Token={}", __FUNCTION__, _pf, token_)
    auto iterator = EventContainer.find(token_);
    if (iterator != EventContainer.end()) {
        iterator->second.emplace(_pf, shared_from_this());
    } else {
        StrategyContainerT container;
        container.emplace(_pf, shared_from_this());
        EventContainer.emplace(token_, container);
    }
}

void Strategy::registerStrategy() {
    StrategyContainer.insert_or_assign(_pf, shared_from_this());
}

bool Strategy::activated() const {
    return _activated;
}

void Strategy::setActivated(bool activated_) {
    _activated = activated_;
}
