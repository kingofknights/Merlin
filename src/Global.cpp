#include "../include/Global.hpp"

#include <nlohmann/json.hpp>

#include "../API/Adaptor.hpp"
#include "../API/OrderUtility.hpp"
#include "../API/Strategy.hpp"
#include "../include/Connection.hpp"
#include "../include/Structure.hpp"

using ContainerPtrContainerT = std::unordered_map<uint64_t, const Connection*>;

extern int					  id;
extern int					  UniqueOrderPacketId;
extern EventContainerT		  EventContainer;
extern AdaptorContainerT	  AdaptorContainer;
extern StrategyContainerT	  StrategyContainer;
static ContainerPtrContainerT ContainerPtrContainer;

void Global::NewConnectionRequested(uint64_t loginID_, const Connection* connection_) {
    ContainerPtrContainer.insert_or_assign(loginID_, connection_);
}

void Global::ConnectionClosed(uint64_t loginID_) {
    auto iterator = ContainerPtrContainer.find(loginID_);
    if (iterator != ContainerPtrContainer.end()) {
        if (iterator->second) {
            delete iterator->second;
        }
        ContainerPtrContainer.erase(iterator);
    }
}

std::string Global::GetStrategyStatus(int pf_) {
    nlohmann::json json;
    json[JSON_ID] = ++id;

    nlohmann::json params;
    params[JSON_PF_NUMBER] = pf_;

    json[JSON_PARAMS] = params;
    return json.dump();
}

void Global::EventReceiver(int token_) {
    const auto iterator = EventContainer.find(token_);
    if (iterator == EventContainer.cend()) return;

    auto& list = iterator->second;
    for (const auto& [_, strategy_] : list) {
        if (strategy_->activated()) {
            strategy_->marketEvent(token_);
        }
    }
}

void Global::AdaptorLoader(ThreadGroupT& threadGroup_, std::string_view dll_, Exchange exchange_) {
    auto dll = std::make_unique<boost::dll::shared_library>(dll_.data(), boost::dll::load_mode::rtld_lazy);
    LOG(WARNING, "dll {} is loaded and has function {}", dll->is_loaded(), dll->has(ENTRY_FUNCTION_NAME))

    // auto getDriver				= dll->get<AdaptorPtrT(ThreadGroupT&)>(ENTRY_FUNCTION_NAME);
    // auto adaptor				= std::invoke(getDriver, threadGroup_);
    // AdaptorContainer[exchange_] = {adaptor, std::move(dll)};
}

void Global::AlogrithmLoader(std::string_view dll_, int pf_, StrategyParameterT param_) {
    auto dll	   = std::make_unique<boost::dll::shared_library>(dll_.data(), boost::dll::load_mode::rtld_lazy);
    auto getDriver = dll->get<StrategyPtrT(int, StrategyParameterT)>(ENTRY_FUNCTION_NAME);
    auto strategy  = std::invoke(getDriver, pf_, param_);
    StrategyContainer.emplace(pf_, strategy);
}

OrderPacketPtrT Global::RegisterOrderPacket(int token_, SideType side_, const std::string& client_, const std::string& algo_, int ioc_, const StrategyPtrT& strategy_) {
    OrderPacketPtrT orderPacket		   = std::make_shared<OrderPacketT>();
    orderPacket->Internal.ResultSetPtr = Lancelot::ContractInfo::GetResultSet(token_);
    orderPacket->Internal.StrategyPtr  = strategy_;
    orderPacket->Internal.AdaptorPtr   = AdaptorContainer[Lancelot::ContractInfo::GetExchange(token_)].AdaptorPtr;
    orderPacket->Internal.UniqueID	   = ++UniqueOrderPacketId;

    OrderUtility::SetIoc(orderPacket, ioc_);
    OrderUtility::SetPrice(orderPacket, 0);
    OrderUtility::SetQuantity(orderPacket, 0);
    OrderUtility::SetTotalQuantity(orderPacket, 0);
    OrderUtility::SetLastPrice(orderPacket, 0);
    OrderUtility::SetLastQuantity(orderPacket, 0);
    OrderUtility::SetOrderNumber(orderPacket, 0);
    OrderUtility::SetSide(orderPacket, side_);
    OrderUtility::SetPF(orderPacket, 0);
    OrderUtility::SetClientCode(orderPacket, client_);
    OrderUtility::SetAlgoID(orderPacket, algo_);
    OrderUtility::SetCurrentOrderStatus(orderPacket, OrderStatus_NONE);
    OrderUtility::SetPreviousOrderStatus(orderPacket, OrderStatus_NONE);

    return orderPacket;
}

void Global::PlaceOrder(const OrderPacketPtrT& orderPacket_, int price_, int quantity_, OrderRequest request_) {
    if (not orderPacket_->Internal.AdaptorPtr) return;

    auto orderStatus = OrderUtility::GetCurrentOrderStatus(orderPacket_);
    OrderUtility::SetCurrentOrderStatus(orderPacket_, OrderStatus_PENDING);

    if (orderPacket_->Internal.AdaptorPtr->execute(orderPacket_, price_, quantity_, request_)) {
        OrderUtility::SetPrice(orderPacket_, price_);
        OrderUtility::SetQuantity(orderPacket_, quantity_);
    } else {
        OrderUtility::SetCurrentOrderStatus(orderPacket_, orderStatus);
    }
}
