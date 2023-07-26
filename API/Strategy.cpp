#include "../include/Global.hpp"
#include "../include/Structure.hpp"
#include "LancelotAPI.hpp"

namespace MerlinShared {
	extern EventContainerT	  _globalEventContainer;
	extern StrategyContainerT _globalStrategyContainer;
}  // namespace MerlinShared

namespace Lancelot::API {
	Strategy::Strategy(int strategy_) : _strategy(strategy_), _activated(false) { LOG(INFO, "New Strategy Requested {}", _strategy) }

	void Strategy::destroy() {
		setActivated(false);
		MerlinShared::_globalStrategyContainer.erase(_strategy);
		for (const auto& [_, container] : MerlinShared::_globalEventContainer) {
			// TODO : Error here

			MerlinShared::_globalEventContainer.erase(_strategy);
		}
	}

	StockPacketPtrT Strategy::getStockPacket(int token_, Side side_, const std::string& client_, const std::string& algo_, int ioc_, bool needEvent_) {
		if (needEvent_) {
			registerForData(token_);
		}
		return Global::RegisterStockPacket(token_, side_, client_, algo_, ioc_, shared_from_this());
	}

	void Strategy::registerForData(int token_) {
		LOG(INFO, "{} Strategy={} Token={}", __FUNCTION__, _strategy, token_)
		auto iterator = MerlinShared::_globalEventContainer.find(token_);
		if (iterator != MerlinShared::_globalEventContainer.end()) {
			iterator->second.emplace(_strategy, shared_from_this());
		} else {
			StrategyContainerT container;
			container.emplace(_strategy, shared_from_this());
			MerlinShared::_globalEventContainer.emplace(token_, container);
		}
	}

	void Strategy::registerSelf() { MerlinShared::_globalStrategyContainer.insert_or_assign(_strategy, shared_from_this()); }

	bool Strategy::activated() const { return _activated; }

	void Strategy::setActivated(bool activated_) { _activated = activated_; }

}  // namespace Lancelot::API