#include "DemoAdaptor.hpp"

#include "../include/Structure.hpp"

DemoAdaptor::DemoAdaptor(ThreadGroupT& threadGroup_) { LOG(ERROR, "{} {}", __FILE__, __FUNCTION__) }

void DemoAdaptor::forwardAssemble(const Lancelot::API::StockPacketPtrT& order_) { LOG(ERROR, "{} {}", __FILE__, __FUNCTION__) }

bool DemoAdaptor::execute(const Lancelot::API::StockPacketPtrT& order_, int price_, int quantity_, Lancelot::API::OrderRequest request_) {
	LOG(ERROR, "{} {}", __FILE__, __FUNCTION__)
	order_->executionReport(Lancelot::API::OrderStatus_ADAPTOR_REJECT);
	return true;
}

void DemoAdaptor::initialization(ThreadGroupT& threadContainer_){LOG(ERROR, "{} {}", __FILE__, __FUNCTION__)}

Lancelot::API::AdaptorPtrT getObject(ThreadGroupT& threadGroup_) {
	LOG(ERROR, "{} {}", __FILE__, __FUNCTION__)
	return std::make_shared<DemoAdaptor>(threadGroup_);
}
