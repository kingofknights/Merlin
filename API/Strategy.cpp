
#include <Gawain.hpp>
#include <Lancelot.hpp>

#include "../include/Connection.hpp"
#include "../include/Global.hpp"
#include "../include/Structure.hpp"

namespace MerlinShared {
extern EventContainerT     _globalEventContainer;
extern StrategyContainerT  _globalStrategyContainer;
extern ArthurMessageQueueT _globalArthurMessageQueue;
}// namespace MerlinShared

namespace Lancelot::API {
Strategy::Strategy(uint32_t address_) : _activated(false), _address(address_), _mutex(PTHREAD_MUTEX_INITIALIZER) {
    LOG(INFO, "New Strategy Requested {}", _address)
    if (pthread_mutex_init(&_mutex, nullptr) != 0) {
        LOG(ERROR, "unable to initialize the mutex for strategy [{}]", _address)
    }
}

Strategy::~Strategy() { pthread_mutex_destroy(&_mutex); }

void Strategy::paramEventManager(const StrategyParamT& param_) {
    if (pthread_mutex_lock(&_mutex)) return;
    paramEvent(param_);
    pthread_mutex_unlock(&_mutex);
}
void Strategy::marketEventManager(uint32_t token_) {
    if (pthread_mutex_trylock(&_mutex)) return;
    marketEvent(token_);
    pthread_mutex_unlock(&_mutex);
}

void Strategy::orderEventManager(int uniqueId_) {
    if (pthread_mutex_lock(&_mutex)) return;
    orderEvent(uniqueId_);
    pthread_mutex_unlock(&_mutex);
}
void Strategy::stopEventManager() {
    if (pthread_mutex_lock(&_mutex)) return;
    stopEvent();
    pthread_mutex_unlock(&_mutex);
}

void Strategy::destroy() {
    setActivated(false);
    MerlinShared::_globalStrategyContainer.erase(_address);
    for (uint32_t token_ : _uniqueToken) {
        const auto iterator = MerlinShared::_globalEventContainer.find(token_);
        if (iterator != MerlinShared::_globalEventContainer.cend()) {
            iterator->second.erase(_address);
        }
    }

    Global::AddressDemangler demangler(getAddress());
    ConnectionPtrT           connection = Global::GetConnection(demangler.getConnectionId());
    if (connection) {
        std::string              status   = Global::GetStrategyStatus(demangler.getStrategyId());
        Lancelot::CommunicationT response = Lancelot::Encrypt(status, demangler.getConnectionId(), Lancelot::ResponseType_UNSUBSCRIBED);
        connection->writeAsync(reinterpret_cast<char*>(&response), sizeof(Lancelot::CommunicationT));
        LOG(INFO, "response {}", status)
    }
}

StockPacketPtrT Strategy::getStockPacket(uint32_t token_, Side side_, const std::string& client_, const std::string& algo_, int ioc_, bool needEvent_) {
    if (needEvent_) {
        registerForData(token_);
        _uniqueToken.emplace(token_);
    }
    return Global::RegisterStockPacket(token_, side_, client_, algo_, ioc_, shared_from_this());
}

void Strategy::registerForData(uint32_t token_) {
    LOG(INFO, "{} Strategy={} Token={}", __FUNCTION__, _address, token_)
    auto iterator = MerlinShared::_globalEventContainer.find(token_);
    if (iterator != MerlinShared::_globalEventContainer.end()) {
        iterator->second.emplace(_address, shared_from_this());
    } else {
        StrategyContainerT container;
        container.emplace(_address, shared_from_this());
        MerlinShared::_globalEventContainer.emplace(token_, container);
    }
}

void Strategy::registerSelf() { MerlinShared::_globalStrategyContainer.emplace(_address, shared_from_this()); }

bool     Strategy::activated() const { return _activated; }
void     Strategy::setActivated(bool activated_) { _activated = activated_; }
uint32_t Strategy::getAddress() const { return _address; }

void Strategy::updateArthur(const StockPacketPtrT& stockPacket_) {
    if (MerlinShared::_globalArthurMessageQueue.write_available()) {
        MerlinShared::_globalArthurMessageQueue.push(stockPacket_);
    }
}

}// namespace Lancelot::API