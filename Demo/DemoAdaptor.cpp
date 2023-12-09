#include "DemoAdaptor.hpp"

#include <Lancelot.hpp>

DemoAdaptor::DemoAdaptor(Lancelot::ThreadGroupT& threadGroup_) { LOG(ERROR, "{} {}", __FILE__, __FUNCTION__) }

void DemoAdaptor::forwardAssemble(const Lancelot::API::StockPacketPtrT& order_) { LOG(ERROR, "{} {}", __FILE__, __FUNCTION__) }

bool DemoAdaptor::execute(const Lancelot::API::StockPacketPtrT& order_, int price_, int quantity_, Lancelot::API::OrderRequest request_) {
    LOG(ERROR, "{} {}", __FILE__, __FUNCTION__)
    Lancelot::API::OrderStatus status;
    switch (request_) {
        case Lancelot::API::OrderRequest_NEW: {
            status = Lancelot::API::OrderStatus_NEW;
            break;
        }
        case Lancelot::API::OrderRequest_MODIFY: {
            status = Lancelot::API::OrderStatus_REPLACED;
            break;
        }
        case Lancelot::API::OrderRequest_CANCEL: {
            status = Lancelot::API::OrderStatus_CANCELLED;
            break;
        }
        default:
            return false;
    }
    order_->setOrderNumber(rand());
    order_->executionReport(status);

    return true;
}

void DemoAdaptor::initialization(Lancelot::ThreadGroupT& threadContainer_){ LOG(ERROR, "{} {}", __FILE__, __FUNCTION__) }

Lancelot::API::AdaptorPtrT getObject(Lancelot::ThreadGroupT& threadGroup_) {
    LOG(ERROR, "{} {}", __FILE__, __FUNCTION__)
    return std::make_shared<DemoAdaptor>(threadGroup_);
}
