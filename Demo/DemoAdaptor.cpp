#include "DemoAdaptor.hpp"

#include "../include/Enums.hpp"
#include "../include/Structure.hpp"

DemoAdaptor::DemoAdaptor(ThreadGroupT& threadGroup_) { LOG(ERROR, "{} {}", __FILE__, __FUNCTION__) }

void DemoAdaptor::forwardAssemble(const OrderPacketPtrT& order_) { LOG(ERROR, "{} {}", __FILE__, __FUNCTION__) }

bool DemoAdaptor::execute(const OrderPacketPtrT& order_, int price_, int quantity_, Lancelot::OrderRequest request_) {
	LOG(ERROR, "{} {}", __FILE__, __FUNCTION__)
	return true;
}

void DemoAdaptor::initialization(ThreadGroupT& threadContainer_){LOG(ERROR, "{} {}", __FILE__, __FUNCTION__)}

AdaptorPtrT getDriver(ThreadGroupT& threadGroup_) {
	LOG(ERROR, "{} {}", __FILE__, __FUNCTION__)
	return std::make_shared<DemoAdaptor>(threadGroup_);
}
