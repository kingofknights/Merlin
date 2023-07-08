#include "Strategy.hpp"

#include "../include/Global.hpp"
#include "../include/Logger.hpp"
#include "../include/Structure.hpp"

namespace details {
	extern EventContainerT	  _globalEventContainer;
	extern StrategyContainerT _globalStrategyContainer;
}  // namespace details

Strategy::Strategy(int pf_, const StrategyParameterT& strategyParameter_) : _pf(pf_), _activated(false) {
	LOG(INFO, "New Strategy Requested {}", _pf)
	LOG(INFO, "Parameters {}", strategyParameter_)
}

void Strategy::terminate() {
	setActivated(false);
	details::_globalStrategyContainer.erase(_pf);
	for (const auto& [_, container] : details::_globalEventContainer) {
		// TODO : Error here
		details::_globalEventContainer.erase(_pf);
	}
}

OrderPacketPtrT Strategy::getOrderPacket(int token_, SideType side_, const std::string& client_, const std::string& algo_, int ioc_) {
	return Global::RegisterOrderPacket(token_, side_, client_, algo_, ioc_, shared_from_this());
}

void Strategy::registerForData(int token_) {
	LOG(INFO, "{} PF={} Token={}", __FUNCTION__, _pf, token_)
	auto iterator = details::_globalEventContainer.find(token_);
	if (iterator != details::_globalEventContainer.end()) {
		iterator->second.emplace(_pf, shared_from_this());
	} else {
		StrategyContainerT container;
		container.emplace(_pf, shared_from_this());
		details::_globalEventContainer.emplace(token_, container);
	}
}

void Strategy::registerStrategy() { details::_globalStrategyContainer.insert_or_assign(_pf, shared_from_this()); }

bool Strategy::activated() const { return _activated; }

void Strategy::setActivated(bool activated_) { _activated = activated_; }
