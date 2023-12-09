#include "../include/Global.hpp"

#include <Gawain.hpp>
#include <Lancelot.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <nlohmann/json.hpp>

#include "../include/Connection.hpp"
#include "../include/Structure.hpp"

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

static constexpr char _globalSharedLibEntryFunctionName[] = "getObject";

using ConnectionPtrContainerT = std::unordered_map<uint32_t, ConnectionPtrT>;

namespace MerlinShared {
extern int                     _globalJsonResponseUniqueId;
extern EventContainerT         _globalEventContainer;
extern AdaptorContainerT       _globalAdaptorContainer;
extern ArthurMessageQueueT     _globalArthurMessageQueue;
extern StrategyContainerT      _globalStrategyContainer;
static ConnectionPtrContainerT _globalConnectionPtrContainer;
extern boost::asio::io_context _globalStrategyThread;
}// namespace MerlinShared

inline constexpr int      _globalAddressDemanglerCode = 10000;
inline constexpr uint64_t _globalTime                 = 1.98e+13;

Global::AddressDemangler::AddressDemangler(uint32_t address_) : _address(address_) {
    _connectionId = address_ / _globalAddressDemanglerCode;
    _strategyId   = address_ % _globalAddressDemanglerCode;
}
Global::AddressDemangler::AddressDemangler(uint32_t connectionId_, uint32_t strategyId_) : _connectionId(connectionId_), _strategyId(strategyId_) {
    _address = _connectionId * _globalAddressDemanglerCode + _strategyId;
}
uint32_t Global::AddressDemangler::getConnectionId() const { return _connectionId; }
uint32_t Global::AddressDemangler::getStrategyId() const { return _strategyId; }
uint32_t Global::AddressDemangler::getAddress() const { return _address; }

boost::posix_time::ptime AsPTime(uintmax_t ns_) {
    return {
        { 1970, 1, 1 },
        boost::posix_time::microseconds(ns_ / 1000)
    };
}

std::string Global::TimeStampToHReadable(time_t time_) {
    std::string time = boost::posix_time::to_iso_extended_string(AsPTime(static_cast<uintmax_t>(time_) + _globalTime));
    boost::algorithm::replace_first(time, "T", " ");
    return time;
}

void Global::NewConnectionRequested(uint32_t loginId_, const ConnectionPtrT& connection_) { MerlinShared::_globalConnectionPtrContainer.insert_or_assign(loginId_, connection_); }

void Global::ConnectionClosed(uint32_t loginId_) { MerlinShared::_globalConnectionPtrContainer.erase(loginId_); }

ConnectionPtrT Global::GetConnection(uint32_t loginId_) {
    const auto iterator = MerlinShared::_globalConnectionPtrContainer.find(loginId_);
    if (iterator != MerlinShared::_globalConnectionPtrContainer.cend()) return iterator->second;
    return nullptr;
}

std::string Global::GetStrategyStatus(uint32_t strategy_) {
    nlohmann::json json;
    json[JSON_ID] = ++MerlinShared::_globalJsonResponseUniqueId;

    nlohmann::json params;
    params[JSON_PF_NUMBER] = strategy_;

    json[JSON_PARAMS] = params;
    return json.dump();
}

void Global::EventReceiver(uint32_t token_) {
    const auto iterator = MerlinShared::_globalEventContainer.find(token_);
    if (iterator == MerlinShared::_globalEventContainer.cend()) return;

    auto& list = iterator->second;
    for (const auto& [_, strategy_] : list) {
        if (strategy_->activated()) {
            MerlinShared::_globalStrategyThread.post([&]() { strategy_->marketEventManager(token_); });
        }
    }
}

void Global::AdaptorLoader(ThreadGroupT& threadGroup_, std::string_view dll_, Lancelot::Exchange exchange_) {
    AdaptorConnectionT connection;
    connection._sharedLibPtr = std::make_unique<boost::dll::shared_library>(dll_.data(), boost::dll::load_mode::rtld_lazy);
    LOG(INFO, "Adaptor request to open is loaded [{}], filename [{}]", connection._sharedLibPtr->is_loaded(), dll_)
    LOG(INFO, "Adaptor [{}] , looking for [{}] found [{}]", dll_, _globalSharedLibEntryFunctionName, connection._sharedLibPtr->has(_globalSharedLibEntryFunctionName))

    if (not connection._sharedLibPtr->is_loaded()) return;
    if (not connection._sharedLibPtr->has(_globalSharedLibEntryFunctionName)) return;

    auto getObject = connection._sharedLibPtr->get<Lancelot::API::AdaptorPtrT(ThreadGroupT&)>(_globalSharedLibEntryFunctionName);

    LOG(INFO, "Adaptor invoking the function [{}]", _globalSharedLibEntryFunctionName)

    connection._adaptorPtr                              = std::invoke(getObject, threadGroup_);
    MerlinShared::_globalAdaptorContainer.at(exchange_) = std::move(connection);
}

bool Global::StrategyLoader(std::string_view name_, uint32_t address_, const Lancelot::API::StrategyParamT& param_) {
    if (not std::filesystem::exists(name_)) {
        LOG(WARNING, "Strategy file not found : {}", name_)
        return false;
    }

    auto dll = std::make_unique<boost::dll::shared_library>(name_.data(), boost::dll::load_mode::rtld_lazy);
    if (not dll->is_loaded()) {
        LOG(WARNING, "Strategy failed to load : {}", name_)
        return false;
    }

    if (not dll->has(_globalSharedLibEntryFunctionName)) {
        LOG(WARNING, "Strategy does not have request entry point : {} {}", name_, _globalSharedLibEntryFunctionName)
        return false;
    }

    auto getObject = dll->get<StrategyPtrT(int, Lancelot::API::StrategyParamT)>(_globalSharedLibEntryFunctionName);
    auto strategy  = std::invoke(getObject, address_, param_);
    auto success   = MerlinShared::_globalStrategyContainer.emplace(address_, strategy);

    return success.second;
}

bool Global::StrategyParamUpdate(uint32_t strategy_, const Lancelot::API::StrategyParamT& param_) {
    const auto iterator = MerlinShared::_globalStrategyContainer.find(strategy_);
    if (iterator != MerlinShared::_globalStrategyContainer.cend()) {
        iterator->second->paramEventManager(param_);
        return true;
    }
    return false;
}

bool Global::StrategyStopRequest(uint32_t address_) {
    const auto iterator = MerlinShared::_globalStrategyContainer.find(address_);
    if (iterator != MerlinShared::_globalStrategyContainer.cend()) {
        iterator->second->stopEventManager();
        return true;
    }
    return false;
}
void Global::StrategyStopCompleted(uint32_t) {}

Lancelot::API::StockPacketPtrT Global::RegisterStockPacket(uint32_t token_, Lancelot::Side side_, const std::string& client_, const std::string& algo_, int ioc_, const StrategyPtrT& strategy_) {
    Lancelot::API::StockPacketPtrT stockPacket = std::make_shared<Lancelot::API::StockPacket>();
    stockPacket->setResultSetPtr(Lancelot::ContractInfo::GetResultSet(token_));
    stockPacket->setStrategyPtr(strategy_);
    stockPacket->setAdaptorPtr(MerlinShared::_globalAdaptorContainer[Lancelot::ContractInfo::GetExchange(token_)]._adaptorPtr);

    stockPacket->setStrategyNumber(strategy_->getAddress());
    stockPacket->setToken(token_);
    stockPacket->setIoc(ioc_);
    stockPacket->setPrice(0);
    stockPacket->setQuantity(0);
    stockPacket->setTotalTradeQuantity(0);
    stockPacket->setLastTradePrice(0);
    stockPacket->setLastTradeQuantity(0);
    stockPacket->setOrderNumber(0);
    stockPacket->setSide(side_);
    stockPacket->setClientCode(client_);
    stockPacket->setAlgoCode(algo_);
    stockPacket->setContractDescription(Lancelot::ContractInfo::GetDescription(token_));
    stockPacket->setCurrentStatus(Lancelot::API::OrderStatus_NONE);
    stockPacket->setPreviousStatus(Lancelot::API::OrderStatus_NONE);

    return stockPacket;
}
auto Global::Serialize(const Lancelot::API::StockPacketPtrT& stockPacket_) -> std::string {
    nlohmann::json param;
    param[JSON_PF_NUMBER]     = stockPacket_->getStrategyNumber();
    param[JSON_UNIQUE_ID]     = stockPacket_->getUniqueClassIdentity();
    param[JSON_TOKEN]         = stockPacket_->getToken();
    param[JSON_QUANTITY]      = stockPacket_->getQuantity();
    param[JSON_FILL_QUANTITY] = stockPacket_->getLastTradeQuantity();
    param[JSON_REMAINING]     = stockPacket_->getQuantity() - stockPacket_->getLastTradeQuantity();
    param[JSON_ORDER_ID]      = stockPacket_->getOrderNumber();
    param[JSON_PRICE]         = static_cast<float>(stockPacket_->getPrice()) / 100.0F;// FIXME : divide with price multiplier
    param[JSON_FILL_PRICE]    = stockPacket_->getLastTradePrice();
    param[JSON_SIDE]          = stockPacket_->getSide();
    param[JSON_CLIENT]        = stockPacket_->getClientCode();
    param[JSON_MESSAGE]       = stockPacket_->getContractDescription();
    param[JSON_TIME]          = Global::TimeStampToHReadable(std::chrono::system_clock::now().time_since_epoch().count());

    nlohmann::json json;
    json[JSON_ID]     = ++MerlinShared::_globalJsonResponseUniqueId;
    json[JSON_PARAMS] = param;
    return json.dump();
}
void Global::ArthurMessageManagerThread(const std::stop_token& stopToken_) {
    while (not stopToken_.stop_requested()) {
        if (MerlinShared::_globalArthurMessageQueue.read_available())
            MerlinShared::_globalArthurMessageQueue.consume_all([&](const ArthurMessageTypeT& arthurMessageType_) {
                std::visit(overloaded{ [&](const Lancelot::API::StockPacketPtrT& stockPacket_) {
                               LOG(WARNING, "{}", Global::Serialize(stockPacket_))
                               Global::AddressDemangler demangler(stockPacket_->getStrategyNumber());
                               const auto               iterator = MerlinShared::_globalConnectionPtrContainer.find(demangler.getConnectionId());
                               if (iterator != MerlinShared::_globalConnectionPtrContainer.cend()) {
                                   Lancelot::ResponseType responseType;
                                   auto                   currentStatus = stockPacket_->getCurrentStatus();
                                   if (currentStatus == Lancelot::API::OrderStatus_NEW) {
                                       responseType = Lancelot::ResponseType_NEW;
                                   } else if (currentStatus == Lancelot::API::OrderStatus_REPLACED) {
                                       responseType = Lancelot::ResponseType_REPLACED;
                                   } else if (currentStatus == Lancelot::API::OrderStatus_CANCELLED) {
                                       responseType = Lancelot::ResponseType_CANCELLED;
                                   }
                                   auto response_ = Lancelot::Encrypt(Serialize(stockPacket_), demangler.getConnectionId(), responseType);
                                   iterator->second->writeAsync(reinterpret_cast<char*>(&response_), sizeof(Lancelot::CommunicationT));
                               }
                           } },
                           arthurMessageType_);
            });
    }
}
