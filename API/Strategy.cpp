#include "../include/Global.hpp"
#include "../include/Structure.hpp"
#include "LancelotAPI.hpp"

namespace details {
	extern EventContainerT	  _globalEventContainer;
	extern StrategyContainerT _globalStrategyContainer;
}  // namespace details

namespace Lancelot::API {
	Strategy::Strategy(int strategy_) : _strategy(strategy_), _activated(false) { LOG(INFO, "New Strategy Requested {}", _strategy) }

	void Strategy::destroy() {
		setActivated(false);
		details::_globalStrategyContainer.erase(_strategy);
		for (const auto& [_, container] : details::_globalEventContainer) {
			// TODO : Error here

			details::_globalEventContainer.erase(_strategy);
		}
	}

	StockPacketPtrT Strategy::getStockPacket(int token_, Side side_, const std::string& client_, const std::string& algo_, int ioc_, bool needEvent_) {
		return Global::RegisterStockPacket(token_, side_, client_, algo_, ioc_, shared_from_this());
	}

	void Strategy::registerForData(int token_) {
		LOG(INFO, "{} PF={} Token={}", __FUNCTION__, _strategy, token_)
		auto iterator = details::_globalEventContainer.find(token_);
		if (iterator != details::_globalEventContainer.end()) {
			iterator->second.emplace(_strategy, shared_from_this());
		} else {
			StrategyContainerT container;
			container.emplace(_strategy, shared_from_this());
			details::_globalEventContainer.emplace(token_, container);
		}
	}

	void Strategy::registerSelf() { details::_globalStrategyContainer.insert_or_assign(_strategy, shared_from_this()); }

	bool Strategy::activated() const { return _activated; }

	void Strategy::setActivated(bool activated_) { _activated = activated_; }

}  // namespace Lancelot::API